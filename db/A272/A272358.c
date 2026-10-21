/*
 * A272358  a(n) = n*(945*n^4 - 3150*n^3 + 4095*n^2 - 2370*n + 496).
 * https://oeis.org/A272358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(945*n^4 - 3150*n^3 + 4095*n^2 - 2370*n + 496)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* For built‑in integer seq_t types (<= 64 bits) use signed int128
       to avoid intermediate overflow; otherwise rely on arbitrary‑precision
       operations which are unsigned but perform the additions first
       to keep all intermediates non‑negative. */
    if (sizeof(seq_t) <= 8) {
        __int128_t nn = (__int128_t)n;        /* cast to signed 128‑bit */
        __int128_t n2 = nn * nn;
        __int128_t n3 = n2 * nn;
        __int128_t n4 = n3 * nn;
        __int128_t res =
            (__int128_t)945 * n4 +
            (__int128_t)4095 * n2 +
            (__int128_t)496 -
            (__int128_t)3150 * n3 -
            (__int128_t)2370 * nn;
        res *= nn;
        return (seq_t)res;
    } else {
        /* Big‑integer case – all operations are unsigned and safe as
           the intermediate stays non‑negative by adding the positive
           terms before subtracting the negative ones. */
        seq_t sn   = n;
        seq_t n2   = sn * sn;
        seq_t n3   = n2 * sn;
        seq_t n4   = n3 * sn;

        seq_t res = (seq_t)945 * n4 + (seq_t)4095 * n2 + (seq_t)496;
        res -= (seq_t)3150 * n3;
        res -= (seq_t)2370 * sn;
        res *= sn;
        return res;
    }
}
