/*
 * A360964  Triangle T(n, k), n > 0, k = 0..n-1, read by rows: T(n, k) is the
 *          least base b >= 2 where the number of digits of n and k are
 *          different.
 * https://oeis.org/A360964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10011, u16 n <= 10011, u32 n <= 10011, u64 n <= 10011,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 107 terms through n = 9995 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A360964 – Triangle T(n,k) read by rows.
   a(n) = minimal base b ≥ 2 such that the number of digits of
   the row number r and column number k are different, where
   r = smallest r with r(r+1)/2 ≥ n  (the row containing the nth term)
   and k = n – (r−1)r/2 – 1                (the column within that row)   */
#include <stdint.h>
#include <stdbool.h>

/* helper: number of digits of x in base b (b >= 2, x >= 0) */
static int digit_len_u64(uint64_t x, uint64_t b)
{
    if (x == 0) return 0;
    int len = 0;
    while (x > 0) {
        ++len;
        x /= b;
    }
    return len;
}

static inline uint64_t floor_isqrt_u64(idx_t n)
{
    // not used; present for completeness
    (void)n; return 0;
}

/* main function */
seq_t a(idx_t n)
{
    /* Find the row r that contains the nth term.
       r = smallest integer with r(r+1)/2 >= n          */
    uint64_t lo = 0, hi = ((uint64_t)1 << 32);          // upper bound for r
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __int128 prod = (__int128)mid * (mid + 1) / 2;
        if (prod >= n) hi = mid;
        else           lo = mid + 1;
    }
    uint64_t r = lo;                                    // row number, 1‑based

    /* index within the row, 0‑based */
    __int128 sumprev = (__int128)(r - 1) * r / 2;
    uint64_t k = (uint64_t)(n - sumprev - 1);           // column number

    /* Special case: k == 0 -> answer is always 2 */
    if (k == 0) return (seq_t)2;

    /* Find the smallest base b such that digit lengths differ */
    for (uint64_t b = 2; ; ++b) {
        int lenr = digit_len_u64(r, b);
        int lenk = digit_len_u64(k, b);
        if (lenr != lenk) return (seq_t)b;
    }
}
