/*
 * A303273  Array T(n,k) = binomial(n, 2) + k*n + 1 read by antidiagonals.
 * https://oeis.org/A303273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(i,2) + i*j + 1 where s =
 *            floor((sqrt(8*n+1)-1)/2), i = n - s*(s+1)/2, j = s - i
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 71 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static inline idx_t antidiag_sum(idx_t n)
{
    /* approximate s via the inverse of (s+1)(s+2)/2 = n+1 */
    double d = sqrt((double)n * 8.0 + 1.0);
    idx_t s = (idx_t)((d - 1.0) / 2.0);

    /* adjust s to satisfy
       s*(s+1)/2 <= n < (s+1)*(s+2)/2   */
    while ( ((unsigned __int128)(s + 1) * (s + 2) / 2) <= n )
        ++s;
    while ( ((unsigned __int128)s * (s + 1) / 2) > n )
        --s;
    return s;
}

seq_t a(idx_t n)
{
    idx_t s  = antidiag_sum(n);                       /* diagonal index */
    idx_t base = (idx_t)(( (unsigned __int128)s * (s + 1) ) / 2); /* terms before s */
    idx_t i = n - base;                               /* row index */
    idx_t j = s - i;                                  /* column index */

    seq_t binom = 0;
    if (i >= 2)
        binom = ((seq_t)i * (i - 1)) / 2;              /* C(i,2) */

    seq_t prod = ((seq_t)i * j);                      /* i*j */

    return binom + prod + 1;                          /* T(i,j) */
}
