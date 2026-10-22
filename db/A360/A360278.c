/*
 * A360278  Determinant of the matrix [L(j+k) + d(j,k)]_{1<=j, k<=n}, where
 *          L(n) denotes the Lucas number A000032(n), and d(j,k) is 1 or 0
 *          according as j = k or not.
 * https://oeis.org/A360278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L(n+1)^2 - 5*(n mod 2) (L = Lucas numbers)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    /* n is 1‑based (n ≥ 1).  fib_pair gives F(n) and F(n+1). */
    seq_t Fn, Fn1;
    fib_pair(n, &Fn, &Fn1);            /* Fn = F(n), Fn1 = F(n+1) */

    /* L(n+1) = 2*F(n) + F(n+1) */
    seq_t L = Fn;
    L += Fn;        /* 2*F(n) */
    L += Fn1;       /* +F(n+1) */

    /* a(n) = L^2 - 5*(n mod 2) */
    seq_t Lsq = L * L;
    seq_t sub = (seq_t)((n & 1) ? 5 : 0);
    return Lsq - sub;
}
