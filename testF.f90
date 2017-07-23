
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

contains

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
