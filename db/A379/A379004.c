/*
 * A379004  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => v_2(i) = v_2(j) and v_5(i) = v_5(j), for all i, j, where v_2
 *          (A007814) and v_5 (A112765) give the 2- and 5-adic valuations of n
 *          respectively.
 * https://oeis.org/A379004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9654 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A379004: the restricted-growth labelling of n by (v_2(n), v_5(n)). The pair
 * (e, f) first occurs at n = 2^e 5^f, so its label is 1 + the number of
 * 2^a 5^b below 2^e 5^f, whatever order the terms are asked in (the original
 * numbered pairs in the order of the calls). */

#include <stdint.h>

typedef unsigned __int128 u128;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t t = (uint64_t)n;
    u128 m = 1;
    while (!(t & 1)) { t >>= 1; m *= 2; }
    while (t % 5 == 0) { t /= 5; m *= 5; }
    uint64_t below = 0;
    for (u128 p5 = 1; p5 < m; p5 *= 5)
        for (u128 v = p5; v < m; v *= 2) ++below;
    return (seq_t)(below + 1);
}
