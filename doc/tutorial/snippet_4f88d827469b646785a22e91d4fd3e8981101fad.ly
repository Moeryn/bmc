% Automatically generated by BMC, the braille music compiler
\version "2.14.2"
\include "articulate.ly"
\header {
  tagline = ""
}
music =
  <<
    \new Staff {
      \time 3/4
      r8 g'( e') r r c''( | % 1
      bes'8 a') r f''( d'' b' | % 2
      c''4) r2 | % 3
    }
  >>

\score {
  \music
  \layout { }
}
\score {
  \unfoldRepeats \articulate \music
  \midi { }
}