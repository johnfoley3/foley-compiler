program semt1;

   a: int;
   b: int;
   c: bool;

begin # semt1

  a := 1;
  b := 2;

  c := a >= b;
  c := a > b;
  c := a <= b;
  c := a < b;
  c := a <> b;
  c := a = b;

  if (c) begin
	c := a >= b;
  end
  else begin
  	c := a >= b;
  end

end; 