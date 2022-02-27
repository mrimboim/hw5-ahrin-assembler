arr
    var 10
    var 20
    var 30
dummy
len
    var 3
sum
    var 0
main
    mov R1 $0
    mov R2 $arr
loop
    cmp R1 len
    jge endLoop
    add sum 0(R2)
    add R1 $1
    add R2 $4
    j loop
endLoop
    ret
uselessLabel
