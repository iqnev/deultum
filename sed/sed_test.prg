PROGRAM SEMANTIC ;
VAR I,I : INTEGER ;
    A : ARRAY [1..10] OF CHAR ;
PROCEDURE P ;
   VAR J : BOOLEAN ;
   BEGIN
      READ (J) ;
      I := I+J-I*I DIV J ;
      IF (J=I) OR NOT (J<>I) THEN A[J]:=' ' ELSE A[I]:='''' ;
      WRITE (I[I],J) ;
      I := I+1
   END ;
BEGIN
   I:='1'; J := 10000 ;
   I := P + 1 ;
   TRUE ;
   WHILE I-10 DO P ;
   WHILE I*'1'-10 DO P ;
   WRITE (J)
END.
