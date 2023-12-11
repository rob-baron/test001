C*********************************************************************72
C
C  A subroutine with 0 parameters
C    declaration in C - void t001_();
C
      Subroutine t001()
      implicit none

      write ( *, '(a)' ) '  hello, world!'

      return
      end

      Subroutine t002(JF)
        LOGICAL  JF

      if( JF .EQV. .FALSE. ) THEN
        write ( *, '(a)' ) '> False'
      ELSE IF ( JF .EQV. .TRUE. ) THEN
        write ( *, '(a)' ) '> True'
      ELSE
        write ( *, '(a)' ) '> ???'
      END IF

      return
      end

      Subroutine t003(JF)
        LOGICAL  JF(10)
        INTEGER COUNT
      DO COUNT = 1, 10
        IF( JF(COUNT) .EQV. .TRUE. ) THEN
          write ( *, '(a)' ) " True "
        ELSE
          write ( *, '(a)' ) " False "
        END IF
        write ( *, '(a)') " "
      END DO

      return
      end
C      SUBROUTINE IRI_SUB(JF,JMAG,ALATI,ALONG,IYYYY,MMDD,DHOUR,
C        &    HEIBEG,HEIEND,HEISTP,OUTF,OARR)