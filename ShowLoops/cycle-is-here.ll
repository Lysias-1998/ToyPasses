define void @test(i32 %n) {
entry:
  ; Check if n is 2077
  %is2077 = icmp eq i32 %n, 2077
  ; If n is 2077, jump to latch, otherwise jump to body
  br i1 %is2077, label %latch, label %body

body:
  %i = phi i32 [ 0, %entry ], [ %i.next, %latch ]
  ; Loop body
  br label %latch

latch:
  %i.in = phi i32 [%i, %body], [0, %entry]
  %i.next = add nsw i32 %i.in, 1
  %cond = icmp slt i32 %i.next, %n
  br i1 %cond, label %body, label %exit

exit:
  ret void
}