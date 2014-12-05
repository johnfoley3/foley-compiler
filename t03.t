program synt2;

   a, b: int;

begin

   # syntax error in expr, this isn't C
   while a := 2 
   begin
      print a;
      b := b + a;
   end;

end;
