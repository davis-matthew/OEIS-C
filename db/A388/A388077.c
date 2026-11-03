/*
 * A388077  Complement of A000332.
 * https://oeis.org/A388077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k if k > binomial(k-n+3,4) and a(n) = k-1 otherwise,
 *            where k = n+floor((24n)^(1/4))-1
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor of the fourth root of an unsigned 128-bit integer.
   The result fits into idx_t (i.e., 64 bits). */
static idx_t floor_fourth_root_u128(unsigned __int128 x)
{
    /* The maximum possible value of 24*n for n < 2^64 is < 2^70,
       so its fourth root < 2^17.5 < 2^18 < 1<<18.  The following
       search range is more than sufficient. */
    idx_t lo = 0;
    idx_t hi = (1ULL << 32);        /* upper bound: hi^4 > x for all x < 2^128 */

    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 val = (unsigned __int128)mid *
                                (unsigned __int128)mid *
                                (unsigned __int128)mid *
                                (unsigned __int128)mid;
        if (val <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Compute the binomial coefficient C(m,4) for m>=0.
   For m<4 the result is 0.  The intermediate product fits in
   unsigned __int128. */
static idx_t binom4_idx(idx_t m)
{
    if (m < 4) return 0;
    unsigned __int128 num =
        (unsigned __int128)m * (m - 1) *
        (unsigned __int128)(m - 2) * (m - 3);
    return (idx_t)(num / 24);
}

seq_t a(idx_t n)
{
    /* 24*n may overflow 64-bit; compute in 128-bit. */
    unsigned __int128 prod24 = (unsigned __int128)24 * n;
    idx_t r = floor_fourth_root_u128(prod24);      /* floor((24n)^(1/4)) */
    idx_t k = n + r - 1;                           /* k = n + floor((24n)^(1/4)) - 1 */

    idx_t m = k - n + 3;                           /* m = k - n + 3 */
    idx_t c = binom4_idx(m);                       /* binomial(m,4) */

    if (k > c)
        return (seq_t)k;
    else
        return (seq_t)(k - 1);
}
