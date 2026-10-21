/*
 * A293144  Number of vertices in a Menger Sponge constructed from a cubic
 *          lattice: a(n) = 20*a(n-1) - 24*A293143(n-1).
 * https://oeis.org/A293144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 24*A293143(n-1) Equivalent 4‑term recurrence:
 *            a(0)=8, a(1)=64, a(2)=896, a(3)=15616,
 *            a(n)=32*a(n-1)-275*a(n-2)+724*a(n-3)-480*a(n-4) (n>3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle small n directly */
    if (n == 0) return (seq_t)8;
    if (n == 1) return (seq_t)64;
    if (n == 2) return (seq_t)896;
    if (n == 3) return (seq_t)15616;

    /* last four terms of the recurrence */
    seq_t t0 = (seq_t)8;
    seq_t t1 = (seq_t)64;
    seq_t t2 = (seq_t)896;
    seq_t t3 = (seq_t)15616;

    idx_t i;
    for (i = 4; i <= n; ++i) {
        seq_t next = (seq_t)32 * t3
                   - (seq_t)275 * t2
                   + (seq_t)724 * t1
                   - (seq_t)480 * t0;

        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }

    return t3;
}
