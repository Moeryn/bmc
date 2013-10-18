% Automatically generated by BMC, the braille music compiler
\version "2.14.2"
\include "articulate.ly"
music =
  <<
    \new PianoStaff <<
      \new Staff {
        \clef "treble"
        \key g \major
        \time 4/4
        \repeat unfold 4 { << {r1}\\{r1}\\{r1} >> } | % 1
        << {r1}\\{r1}\\{d'2\mordent -3 d'4. c'16 d'} >> | % 2
        << {r1}\\{r1}\\{e'4 c' a c'-4} >> | % 3
        << {r1}\\{r1}\\{a4 d' d c'} >> | % 4
        << {r1}\\{r1}\\{b4 a8 b g  b a-2 c'} >> | % 5
        << {g''2\mordent -3 g''4. fis''16 g''}\\{r1} >> | % 6
        << {a''4 fis'' d'' fis''-4}\\{r1} >> | % 7
        << {b'4 e''-5 e' d''-4}\\{r1} >> | % 8
        << {cis''4 b'8 cis'' a' b' cis'' a'}\\{r1} >> | % 9
        << {d''4-5 d' d''2~}\\{a'2-2 a'4. g'16 a'} >> | % 10
        << {d''4 d' d''2~}\\{b'4 g' e' g'} >> | % 11
        << {d''2 cis''}\\{e'4 a'-2  a  g'-1} >> | % 12
        << {d''1}\\{fis'4 e'8 fis' d'2 } >> \bar ":|:" % 13
        << { fis''2\mordent -3 fis''4. e''16 fis''}\\{r1} >> | % 14
        << {g''4-5 d'' b' d''-3}\\{r1} >> | % 15
        << {c''4 f'' f' e''-4}\\{r1} >> | % 16
        << {dis''4 cis''8 dis'' b' cis'' dis'' b'}\\{r1} >> | % 17
        << {e''4-5 e' e''2~}\\{b'2-2 b'4. a'16 b'} >> | % 18
        << {e''4 e' e''2~}\\{c''4 a' fis'-1 a'} >> | % 19
        << {e''2 dis''}\\{fis'4 b'  b  a'-2} >> | % 20
        << {e''2~ e''8 d''-5 cis'' b'}\\{g'4 fis'8-2 g'-2 e'2~} >> | % 21
        << {a'4 g''2 f''8 e''}\\{ e'4 r a'2~} >> | % 22
        << {f''2.-5 e''8 d''}\\{a'4 d'' b' g'~} >> | % 23
        << {e''2.-5 fis''8-4 g''}\\{g'4 c''8 b' c''2~-1} >> | % 24
        << {a''8-4 b'' g'' a'' fis''4 a''~-5}\\{c''2. c''4-2} >> | % 25
        << {a''4 g''8 fis'' g''4-5 b'~-4}\\{b'2 r4 g'} >> | % 26
        << {b'4 a'8 b' c''2~}\\{e'2. a'8-4 g'} >> | % 27
        <c'' fis'>4 <b'-4 g'~-1> <<{a'2-5}\\{ g'4 fis'4-3 }>> | % 28
        << {g'1}\\{g'8 d' c' d' b2 } >> \bar ":|" % 29
      }
      \new Staff {
        \clef "bass"
        \key g \major
        \time 4/4
        g2\mordent -2 g4. fis16 g | % 1
        a4 fis d fis-2 | % 2
        e4 a a, g-1 | % 3
        fis4 e8 fis d e-1 c d | % 4
        b,8 d e fis-2 g4 b, | % 5
        c2 r4 a,-2 | % 6
        fis,2 r4 d, | % 7
        g,4-3 d b,-5 d-5 | % 8
        << {g2-4 r4 e}\\{b8-2 a b4 e'2~} >> | % 9
        << {fis2 r4 d}\\{e'4 a d'2~} >> | % 10
        << {g2 r4 e}\\{d'4 b  e'2~} >> | % 11
        << {a2-"3-4" r4 g-5}\\{ e'4  d' cis' e'} >> | % 12
        << {fis2-4 r4 d-3}\\{a8 b a g-1 fis2} >> | % 13
        << {g,4 b,-2 g, e,}\\{g2 r} >> | % 14
        << {a,2-"3-5" r4 a,}\\{r4 e a2~} >> | % 15
        << {d1-"3-5"}\\{a8 a g a fis2 } >> \bar ":|:" % 16
        << { d8 e fis g a4 c-4}\\{r1} >> | % 17
        << {b,8 d-4 e fis g a-2 b4~}\\{r1} >> | % 18
        << {b4 a8 gis a2~}\\{r1} >> | % 19
        << {a4 fis b a}\\{r1} >> | % 20
        << {g8 a b a g fis g e}\\{r1} >> | % 21
        << {a8 b c'-2 b-1 a g a fis}\\{r1} >> | % 22
        << {b8 a g a b a b b,}\\{r1} >> | % 23
        << {e8-1 b,-2 a, b,-1 g,-3 fis, g, e,}\\{r1} >> | % 24
        << {cis2 cis4. b,16 cis}\\{r1} >> | % 25
        << {d4 b, g, b,}\\{r1} >> | % 26
        << {c4 e a,-5 g}\\{r1} >> | % 27
        << {fis4 e8 fis d e fis d}\\{r1} >> | % 28
        << {g4-5 a b g}\\{d'2 d'4. c'16 d'-1} >> | % 29
        << {c2 r4 a-2}\\{ e'4  c' a-3 c'} >> | % 30
        << {d2 r4 d}\\{a4 d' d c'} >> | % 31
        << {g2-4 g,}\\{b4 a8 b g2 } >> \bar ":|" % 32
      }
    >>
  >>

\score {
  \music
  \layout { }
}
\score {
  \unfoldRepeats \articulate \music
  \midi { }
}
