define i32 @test(i32 %n) {
entry:
  %i = add nsw i32 %n, 1
  br label %exit

exit:
  ret i32 %i
}