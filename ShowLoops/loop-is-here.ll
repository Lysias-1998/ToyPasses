define void @test(i32 %n) {
entry:
  br label %body

body:
  %i = phi i32 [ 0, %entry ], [ %i.next, %latch ]
  ; Loop body
  br label %latch

latch:
  %i.next = add nsw i32 %i, 1
  %cond = icmp slt i32 %i.next, %n
  br i1 %cond, label %body, label %exit

exit:
  ret void
}