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

C t002 - PrintBool
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

C Print JF bool array
      Subroutine t003(JF)
        LOGICAL  JF(10)
        INTEGER COUNT
      DO COUNT = 1, 10
        CALL t002(JF(COUNT))
      END DO

      write ( *, '(a)') " "

      return
      end


      Subroutine t004(JF, JMAG)
        LOGICAL JF(10)
      CALL t003(JF)
      write (*, '(a,i10)') 'JMAG = ', JMAG
      return
      end

      Subroutine t005(JF, JMAG, ALATI, ALONG)
C        REAl ALATI,ALONG
        LOGICAL JF(10)
      CALL t004(JF, JMAG)
      write (*, '(a,F8.3,a,F8.3)') 'ALATI = ', ALATI, ' ALONG = ', ALONG
      return
      end

      Subroutine t006(ALATI, ALONG)
        REAl*8 ALATI,ALONG
        LOGICAL JF(10)
      CALL t004(JF, JMAG)
      write (*, '(a,F8.3,a,F8.3)') 'ALATI = ', ALATI, ' ALONG = ', ALONG
      return
      end

      Subroutine t007(OUTF)
        REAL OUTF(20,100)
        INTEGER x, y
      DO x = 1,  100
        DO y = 1, 20
            OUTF(y,x) = x + (y * 0.01)
        END DO
      END DO
      RETURN
      END
C      SUBROUTINE IRI_SUB(JF,JMAG,ALATI,ALONG,IYYYY,MMDD,DHOUR,
C        &    HEIBEG,HEIEND,HEISTP,OUTF,OARR)