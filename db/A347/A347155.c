/*
 * A347155  Sum of divisors of nontriangular numbers.
 * https://oeis.org/A347155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma_of(A014132(n)) (sum of divisors of the nth
 *            non‑triangular number)
 * Range:     u8 n <= 7, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_sqrt, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for n less than 1 we return 0 safely. */
    if (n < 1u) return (seq_t)0;

    /* Compute the nth non‑triangular number:
       m(n) = n + ceil( (sqrt(8*n+1) - 1) / 2 )      */
    idx_t e64 = (idx_t)8 * n + (idx_t)1;           /* 8*n + 1   */
    seq_t s = floor_sqrt((seq_t)e64);              /* floor(sqrt(8*n+1)) */

    seq_t add;
    if (s * s == (seq_t)e64)        /* perfect square case */
        add = (s - (seq_t)1) / (seq_t)2;
    else                            /* non‑perfect square case */
        add = (s + (seq_t)1) / (seq_t)2;

    seq_t m = (seq_t)n + add;       /* nth non‑triangular number */

    /* Return its sum of divisors. */
    return sigma_of((idx_t)m);
}
