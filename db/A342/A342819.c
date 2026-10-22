/*
 * A342819  Table read by ascending antidiagonals: T(k, n) is the number of
 *          distinct values of the magic constant in a perimeter-magic k-gon
 *          of order n.
 * https://oeis.org/A342819
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A342819: Table read by ascending antidiagonals.
   a(n) = T(k,n') where T(k,n') = k*(n'-2) + ((k mod 2)-1)*(n' mod 2) + 1.
   The pairs (k,n') are ordered by increasing k+n' and, for each sum, by
   decreasing k (largest k first). The offset of a(3) is 0. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 3 */
    if (n < 3)
        return 0;

    /* offset of the term in the anti-diagonal order, 0‑based */
    idx_t offset = n - 3;

    /* --- Find t: smallest integer with t*(t+1)/2 > offset --- */
    idx_t low  = 0;
    idx_t high = offset + 2;                 /* ensure high > t */
    if (high <= offset)                      /* overflow guard */
        high = UINT64_MAX;

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= offset)          /* need larger t */
            low = mid + 1;
        else
            high = mid;
    }
    idx_t t = low;                         /* t >= 1 */

    /* --- Compute index within diagonal t --- */
    __uint128_t prev_tri = (__uint128_t)t * (t - 1) / 2;  /* T_{t-1} */
    idx_t idx = offset - (idx_t)prev_tri;                /* 0‑based */

    /* --- Recover (k , n') ---
         For diagonal sum d = t + 5:  k runs from d-3 down to 3,
         n' = d - k.  Thus              k = d-3-idx = t+2-idx,
                                           n' = 3+idx.             */
    idx_t k   = t + 2 - idx;
    idx_t nv  = 3 + idx;                     /* rename n' to nv */

    /* --- Evaluate T(k,n') using the formula --- */
    if ((k & 1ULL) == 0ULL) {                /* k even  -> use ((k mod 2)-1) = -1 */
        __uint128_t prod = (__uint128_t)k * (nv - 2);
        return (seq_t)prod + 1 - (nv & 1);
    } else {                                 /* k odd   -> ((k mod 2)-1) = 0  */
        __uint128_t prod = (__uint128_t)k * (nv - 2);
        return (seq_t)prod + 1;
    }
}
