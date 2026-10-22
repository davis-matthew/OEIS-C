/*
 * A390491  Array of Fibonacci polynomial linear combinations read by
 *          antidiagonals: T(m,r) = F_m(r) + 2*F_{m-1}(r) + F_{m-2}(r) where
 *          F_k(r) is the k-th Fibonacci polynomial evaluated at r (m >= 2, r
 *          >= 1).
 * https://oeis.org/A390491
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 *  OEIS A390491   A390491: Array of Fibonacci polynomial linear combinations read by antidiagonals
 *  Formula: a(n) is read from the array T(m,r) by antidiagonals with
 *           T(m,r) = F_m(r) + 2*F_{m-1}(r) + F_{m-2}(r)   (m>=2, r>=1),
 *           where F_k(r) satisfies  F_0(r)=0,  F_1(r)=1,
 *           and  F_{k+1}(r)=F_k(r)+r*F_{k-1}(r).
 *  Offset: 2   (so a(2) is the first term)
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* sequence is defined only for n >= 2 */
    if (n < 2)
        return 0;

    /* 0‑based index inside the antidiagonal‑read sequence */
    idx_t p = n - 2;

    /* Find the antidiagonal number d such that
       sum_{i=1}^{d-1} i <= p < sum_{i=1}^{d} i .
       The sum of the first d natural numbers is d*(d+1)/2.     */
    idx_t d = 1;
    while ((d * (d + 1) / 2) <= p)
        ++d;

    /* Position inside this antidiagonal (0‑based) */
    idx_t prev_sum = (d - 1) * d / 2;
    idx_t pos = p - prev_sum;

    /* Corresponding (m,r) coordinates:
       T(2, d), T(3, d-1), T(4, d-2), ...  =>  m = 2 + pos, r = d - pos.      */
    idx_t m = 2 + pos;
    idx_t r = d - pos;

    /* Compute Fibonacci polynomials F_k(r) up to k = m.
       m is at most about 142 for n <= 10000, so a small array suffices. */
    seq_t f[150];
    f[0] = 0;
    f[1] = 1;
    for (idx_t i = 2; i <= m; ++i)
    {
        f[i] = f[i - 1] + (seq_t)r * f[i - 2];
    }

    /* T(m,r) = F_m + 2*F_{m-1} + F_{m-2} */
    seq_t value = f[m] + 2 * f[m - 1] + f[m - 2];
    return value;
}
