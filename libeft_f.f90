module libeft

  interface
     subroutine twoProdSum_s(a,b,c, x,e)
       implicit none
       real, intent(in) :: a, b, c
       real, intent(out):: x, e
     end subroutine twoProdSum_s
  end interface

end module libeft
