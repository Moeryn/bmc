% Automatically generated by BMC, the braille music compiler
\version "2.14.2"
\include "articulate.ly"
\header {
  tagline = ""
}
music =
  <<
    \new Staff {
      \time 6/8
      c'8 e' g' a' c' a' | % 1
      c'8 bes' c' a' c' a' | % 2
      g'2. | % 3
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
