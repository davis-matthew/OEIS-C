/*
 * A366096  Expansion of (1/x) * Series_Reversion( x*(1+x-x^3)/(1+x)^3 ).
 * https://oeis.org/A366096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binom(n+k,k) * binom(2n-k+2,
 *            n-3k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS definition uses the utility function binom(a,b) which
   is declared in the build's utility library.  Include its header
   so the compiler knows the prototype.  All arithmetic is
   performed in the seq_t domain, which may be an arbitrary‑precision
   integer type, so intermediate overflows are safe. */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3u;          /* floor(n / 3) */
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term1 = binom(n + k, k);                     /* binom(n+k,k) */
        idx_t m   = 2 * n - k + 2;                          /* 2n-k+2 */
        idx_t r   = n - 3 * k;                             /* n-3k   */
        seq_t term2 = binom(m, r);                         /* binom(2n-k+2, n-3k) */
        sum += term1 * term2;                              /* accumulate */
    }
    return sum / ((seq_t)n + 1); /* divide by (n+1) */
}
