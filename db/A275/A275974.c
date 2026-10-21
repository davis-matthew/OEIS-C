/*
 * A275974  Partial sums of the Jeffreys binary sequence A275973.
 * https://oeis.org/A275974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 596, u16 n <= 9957, u32 n <= 9957, u64 n <= 9957,
 *            u128 n <= 9957, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275974 (partial sums of Jeffreys binary sequence A275973)
   The value a(n) is constant on intervals [4^k, 2·4^k] and then
   increases by 1 on (2·4^k, 4^{k+1}].  Precisely:
     if 4^k ≤ n ≤ 2·4^k,   a(n) = 1 + 2·(4^k – 1)/3
     if 2·4^k < n ≤ 4^{k+1}, a(n) = n – (4^{k+1} – 1)/3
   offset: a(1) = 1.
   The function works for any unsigned 64‑bit index n ≥ 1. */

#include <stdint.h>

seq_t a(idx_t n) {
    /* handle the (unlikely) offset */
    if (n == 0) return 0;

    /* find the largest k with 4^k ≤ n, using 128‑bit to avoid overflow */
    unsigned __int128 pow4k  = 1;   /* 4^k   */
    unsigned __int128 pow4k1 = 4;   /* 4^{k+1} */
    while (pow4k1 <= n) {
        pow4k  = pow4k1;
        pow4k1 <<= 2;               /* multiply by 4 */
    }

    unsigned __int128 two_pow4k = pow4k << 1;   /* 2·4^k */

    if ((unsigned __int128)n <= two_pow4k) {
        /* a(n) = 1 + 2·(4^k – 1)/3 */
        unsigned __int128 v = (unsigned __int128)2 * ((pow4k - 1) / 3) + 1;
        return (seq_t)(uint64_t)v;
    } else {
        /* a(n) = n – (4^{k+1} – 1)/3 */
        unsigned __int128 sub = (pow4k1 - 1) / 3;
        unsigned __int128 v = (unsigned __int128)n - sub;
        return (seq_t)(uint64_t)v;
    }
}
