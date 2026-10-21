/*
 * A256243  Smallest positive integer m such that n + 2m is a square.
 * https://oeis.org/A256243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (s^2 - n)/2, where s is the smallest integer ≥ sqrt(n) *
 *            with the same parity as n; if s^2 == n then s += 2 to ensure m >
 *            0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(n)) for 0 ≤ n < 2^64. */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = (1ULL << 32);   /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    idx_t s = isqrt_u64(n);
    /* make s the ceil of sqrt(n) */
    if ((unsigned __int128)s * s < (unsigned __int128)n) s++;

    /* enforce same parity as n */
    if ((s & 1) != (n & 1)) s++;

    /* if we landed exactly on a square, bump by 2 to keep m>0 */
    unsigned __int128 prod = (unsigned __int128)s * s;
    if (prod == (unsigned __int128)n) s += 2;

    /* recompute once more if s changed */
    prod = (unsigned __int128)s * s;
    unsigned __int128 diff = prod - (unsigned __int128)n;
    seq_t m = (seq_t)(diff / 2);
    return m;
}
