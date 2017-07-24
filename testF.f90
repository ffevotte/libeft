
program testF
  implicit none
  character(len=32) :: arg

  call get_command_argument(1, arg)
  if (arg == "dot") then
     call get_command_argument(2, arg)

     if (arg == "std") then
        call readDot0()
     end if

     if (arg == "comp") then
        call readDot()
     end if
  end if

  if (arg == "perf") then
     call checkPerf()
  end if

contains

  subroutine checkPerf ()
    implicit none
    integer :: i, N, Nmax, nops, nloops
    real*8, allocatable :: x(:), y(:)
    real*8 :: keep, begin, end, elapsed, elapsed0, flops, flops0

    write(*,1001) "# N          flops_comp   flops_base"
1001 format (a)

    Nmax = 1e8
    N = 50
    do while (N < Nmax)
       keep = 0

       allocate(x(N))
       allocate(y(N))

       do i = 1,N
          x(i) = 0.1 * i
          y(i) = 0.01 * i * i
       end do

       nops = 2*N
       nloops = int(1e9) / nops
       if (nloops < 1) then
          nloops = 1
       end if

       call cpu_time (begin)
       do i = 1,nloops
          keep = keep + dot(x,y,N)
       end do
       call cpu_time (end)
       elapsed = end - begin
       flops = real(nops) * real(nloops) / elapsed

       call cpu_time (begin)
       do i = 1,nloops
          keep = keep + dot0(x,y,N)
       end do
       call cpu_time (end)
       elapsed0 = end - begin
       flops0 = real(nops) * real(nloops) / elapsed0

       write(*,1000) real(N), flops, flops0, keep
1000   format (e12.6, " ", e12.6, " ", e12.6, " ", e12.6)

       deallocate(x)
       deallocate(y)

       N = int(N*1.5)
    end do

  end subroutine checkPerf

  subroutine readDot0 ()
    implicit none
    integer :: i, n
    real*8 :: d
    real*8, allocatable :: x(:),y(:)

    read(*,*) n
    allocate (x(N))
    allocate (y(N))

    do i = 1,n
       read(*,*) x(i),y(i)
    end do

    d = dot0(x, y, n)
    print *, d
  end subroutine readDot0

  subroutine readDot ()
    implicit none
    integer :: i, n
    real*8 :: d
    real*8, allocatable :: x(:),y(:)

    read(*,*) n
    allocate (x(N))
    allocate (y(N))

    do i = 1,n
       read(*,*) x(i),y(i)
    end do

    d = dot(x, y, n)
    print *, d
  end subroutine readDot

  function dot0 (x, y, N) result(acc)
    implicit none
    integer :: N, i
    real*8 :: acc
    real*8, allocatable :: x(:), y(:)

    acc = 0
    do i = 1,n
       acc = acc + x(i) * y(i)
    end do
  end function dot0

  function dot (x, y, N) result(acc)
    use libeft
    implicit none
    integer :: N, i
    real*8 :: acc, accErr, err
    real*8, allocatable :: x(:), y(:)

    acc = 0
    accErr = 0
    do i = 1,n
       call twoProdSum_d (x(i), y(i), acc, acc, err)
       accErr = accErr + err
    end do
    acc = acc + accErr
  end function dot
end program testF
