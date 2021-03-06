// Copyright (C) 2012, 2013  Mario Lang <mlang@delysid.org>
//
// Distributed under the terms of the GNU General Public License version 3.
// (see accompanying file LICENSE.txt or copy at
//  http://www.gnu.org/licenses/gpl-3.0-standalone.html)

#ifndef BMC_AST_AST_HPP
#define BMC_AST_AST_HPP

#include <cmath>
#include <type_traits>
#include <vector>

#include <boost/mpl/remove.hpp>
#include <boost/optional.hpp>
#include <boost/variant/variant.hpp>

#include "bmc/music.hpp"
#include "bmc/braille_music.hpp"

namespace music { namespace braille {

/**
 * \brief Data types required to store parse result.
 *
 * Some of the missing values are filled in by the compiler translation unit
 * right now.  However, it is likely that we will need two different ways of
 * storing our data, one form immediately after parsing, and another as a result
 * of the necessary transformations on top of that.
 */
namespace ast {

enum value
{
  whole_or_16th, half_or_32th, quarter_or_64th, eighth_or_128th, unknown
};

struct locatable
{
  ssize_t id = -1;
  int line, column;
};

enum class notegroup_member_type: uint8_t {
  none, begin, middle, end
};

enum class slur_member_type: uint8_t {
  none, begin, middle, end
};

/** \brief Storage for rhythmic values.
 *
 * A rhythmic value consists of an ambiguous value type and the number of
 * augmentation dots.  The unambiguous value type is filled in after the parse
 * step by the music::braille::compiler.
 */
struct rhythmic_data
{
  ast::value ambiguous_value = unknown;
  unsigned dots = 0;
  rational type = {}; // filled in by value_disambiguation.hpp
  rational factor = rational{1};
  notegroup_member_type notegroup_member = notegroup_member_type::none; // filled in by value_disambiguation.hpp
  slur_member_type slur_member = slur_member_type::none;
  std::vector<rational> tuplet_begin;
  unsigned tuplet_end = 0;
};

/** \brief Base class for everything that implies a rhythmic value.
 */
class rhythmic
{
protected:
  rhythmic() {}
  virtual ~rhythmic() {}
public:
  virtual rational as_rational() const = 0;
  virtual unsigned get_dots() const = 0;
};

struct slur : locatable
{
  enum type { single, cross_staff };
  type value;
};

struct tie : locatable
{
  enum type { single, chord, arpeggio };
  type value;
};

struct pitched
{
  boost::optional<accidental> acc;
  boost::optional<unsigned> octave_spec;
  unsigned octave;
  diatonic_step step;
  int alter;
  boost::optional<ast::tie> tie;
};

struct stem : rhythmic
{
  rational type;
  unsigned dots;
  boost::optional<ast::tie> tied;

  rational as_rational() const
  { return type * 2 - type / pow(2, dots); }
  virtual unsigned get_dots() const
  { return dots; }
};

struct note : locatable, rhythmic_data, rhythmic, pitched
{
  std::vector<articulation> articulations;
  std::vector<slur> slurs;
  fingering_list fingers;
  std::vector<stem> extra_stems;

  note(): locatable(), rhythmic_data(), pitched() {}
  virtual rational as_rational() const
  { return type * augmentation_dots_factor(dots) * factor; }
  virtual unsigned get_dots() const
  { return dots; }
};

struct rest : locatable, rhythmic_data, rhythmic
{
  rest(): locatable(), rhythmic_data(), whole_measure(false) {}
  bool whole_measure; // filled in by disambiguate.hpp
  virtual rational as_rational() const
  { return type * augmentation_dots_factor(dots) * factor; }
  virtual unsigned get_dots() const
  { return dots; }
};

struct interval : locatable, pitched
{
  music::interval steps;
  fingering_list fingers;
};

struct chord_tied {};

struct chord : locatable, rhythmic
{
  note base;
  std::vector<interval> intervals;
  boost::optional<chord_tied> all_tied;

  virtual rational as_rational() const
  { return base.as_rational(); }
  virtual unsigned get_dots() const
  { return base.get_dots(); }
};

/** The moving-note device, although infrequently employed,
 *  is chiefly useful for vocal music and keyboard settings of
 *  hymns.  Complications of fingering, phrasing and nuances render it
 *  unsuitable for instrumental music in general.
 */
struct moving_note : locatable, rhythmic
{
  note base;
  std::vector<interval> intervals;

  virtual rational as_rational() const
  { return base.as_rational(); }
  virtual unsigned get_dots() const
  { return base.get_dots(); }
};

struct value_distinction : locatable
{
  enum type { distinct, large_follows, small_follows };
  type value;
};

struct hyphen : locatable
{};

struct simile : locatable
{
  boost::optional<unsigned> octave_spec;
  unsigned count;

  rational duration;                    // Filled by value_disambiguation
};

enum barline { begin_repeat, end_repeat, end_part };

class tuplet_start : public locatable
{
  bool simple_triplet_;
  bool doubled_;
  unsigned number_;
public:
  tuplet_start(bool doubled = false)
  : simple_triplet_{true}, doubled_{doubled} {}
  tuplet_start(unsigned number, bool doubled)
  : simple_triplet_{false}, doubled_{doubled}, number_{number} {}

  unsigned number() const { return simple_triplet_? 3: number_; }
  bool simple_triplet() const { return simple_triplet_; }
  bool doubled() const { return doubled_; }
};

class clef : public locatable
{
public:
  enum class type { G, C, F };
private:
  type sign_;
  boost::optional<unsigned> line_;
  boost::optional<unsigned> staff_;
public:
  clef(): sign_{type::G} {}
  clef(type sign): sign_{sign} {}
  clef(type sign, unsigned line): sign_{sign}, line_{line} {}

  type sign() const { return sign_; }
  unsigned line() const {
    if (line_) return *line_;
    switch(sign_) {
    case type::G: return 2;
    case type::F: return 4;
    case type::C: return 3;
    }
  }
  clef &staff(boost::optional<unsigned> const &n) {
    staff_ = n;
    return *this;
  }
  boost::optional<unsigned> const &staff() const { return staff_; }
};

typedef boost::variant< note, rest, chord, moving_note
		      , value_distinction, hyphen, tie, tuplet_start
                      , hand_sign, clef, simile, barline
                      >
        sign;

struct partial_voice : locatable, std::vector<sign> {};
struct partial_measure : locatable, std::vector<partial_voice> {};
struct voice : locatable, std::vector<partial_measure> {};

struct measure : locatable
{
  boost::optional<unsigned> ending;
  std::vector<voice> voices;
};

struct key_and_time_signature : locatable
{
  key_signature key;
  time_signature time;
};

typedef boost::variant<measure, key_and_time_signature> paragraph_element;
typedef std::vector<paragraph_element> paragraph;

struct measure_specification : locatable
{
  typedef unsigned number_type;
  number_type number;
  boost::optional<number_type> alternative;
};

struct measure_range : locatable
{
  measure_specification first;
  measure_specification last;
};

struct section : locatable
{
  typedef unsigned number_type;
  boost::optional<key_and_time_signature> key_and_time_sig;
  boost::optional<number_type> number;
  boost::optional<measure_range> range;
  std::vector<paragraph> paragraphs;
};

typedef std::vector<section> part;

/** These data structures correspond to an unrolled representation which
 *  replaces partial measure simile with the musical material they represent.
 */
namespace unfolded {

typedef boost::make_variant_over
        < boost::mpl::remove
        < boost::mpl::remove
        < boost::mpl::remove
        < ast::sign::types
        , value_distinction>::type
        , hyphen>::type
        , simile>::type
        >::type sign;

struct partial_voice : locatable, std::vector<sign>
{
  partial_voice()
  : std::vector<sign>()
  {}
  partial_voice(const_iterator begin, const_iterator const &end)
  : std::vector<sign>(begin, end)
  {}
};
struct partial_measure : locatable, std::vector<partial_voice> {};
struct voice : locatable, std::vector<partial_measure> {};

struct measure : locatable
{
  boost::optional<unsigned> ending;
  std::vector<voice> voices;
  unsigned count;

  measure()
  : count(1)
  {}
};

typedef boost::variant<measure, ast::key_and_time_signature> staff_element;
struct staff : std::vector<staff_element> {};
struct part : std::vector<staff> {};

}

struct score {
  key_signature key_sig;
  std::vector<time_signature> time_sigs;
  std::vector<part> parts;
  std::vector<unfolded::part> unfolded_part;
};

}}}

#endif
