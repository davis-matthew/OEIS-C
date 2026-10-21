/*
 * A288658  Numbers whose squares have an even number of digits.
 * https://oeis.org/A288658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose squares have an even number of decimal digits.
   The i‑th term is the i‑th positive integer x for which the decimal
   representation of x^2 contains an even number of digits. */

#include <stdint.h>
#include <stddef.h>

/* Computes 10^e for small e. */
static idx_t pow10_idx(idx_t e)
{
    idx_t r = 1;
    while (e--) r *= 10;
    return r;
}

/* Floor of sqrt(n) for idx_t (uint64_t) using binary search with 128‑bit
   multiplication to avoid overflow. */
static idx_t floor_sqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t lo = 1, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Ceil of sqrt(10^(2*d+1)), i.e. the first integer whose square has
   2*(d+1) decimal digits. */
static idx_t ceil_sqrt10_power_d(idx_t d)
{
    idx_t pow = pow10_idx(2 * d + 1);            /* 10^(2d+1) */
    idx_t s   = floor_sqrt_u64(pow);             /* floor(sqrt(...)) */
    if ((unsigned __int128)s * s < pow) s++;      /* ceil if needed */
    return s;
}

/* Sequence a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1 */

    idx_t idx = n;         /* 1‑based index within sequence */
    idx_t cum = 0;         /* cumulative number of terms processed */

    for (idx_t d = 0; ; ++d) {
        idx_t L = ceil_sqrt10_power_d(d);          /* first of this block */
        idx_t U = pow10_idx(d + 1) - 1;            /* last of this block */
        idx_t count = (U >= L) ? (U - L + 1) : 0;  /* number in block */

        idx_t new_cum = cum + count;
        if (idx <= new_cum) {                      /* target lies here */
            idx_t offset = idx - cum - 1;           /* 0‑based in block */
            return (seq_t)(L + offset);
        }
        cum = new_cum;
    }
}
