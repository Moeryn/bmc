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
      b'8 b' b' b' b' b' | % 1
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
