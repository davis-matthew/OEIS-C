/*
 * A351874  Number of subsets of {1,2,...,n} such that no two elements differ
 *          by 1, 3, or 4.
 * https://oeis.org/A351874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n)=a(n-1)+a(n-5)+a(n-7) for n≥7, a(0)=1, a(1)=2, a(2)=3,
 *            a(3)=5, a(4)=7, a(5)=9, a(6)=12
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, not strictly needed but harmless */

seq_t a(idx_t n)
{
    /* base values for indices 0..6 */
    const seq_t base[7] = {
        (seq_t)1, /* a(0) */
        (seq_t)2, /* a(1) */
        (seq_t)3, /* a(2) */
        (seq_t)5, /* a(3) */
        (seq_t)7, /* a(4) */
        (seq_t)9, /* a(5) */
        (seq_t)12 /* a(6) */
    };

    if (n < 7)
        return base[n];

    /* rolling buffer holding a[i-1], a[i-2], ..., a[i-7] */
    seq_t prev[7];

    /* initialize buffer for i = 7 */
    prev[0] = base[6]; /* a(6) */
    prev[1] = base[5]; /* a(5) */
    prev[2] = base[4]; /* a(4) */
    prev[3] = base[3]; /* a(3) */
    prev[4] = base[2]; /* a(2) */
    prev[5] = base[1]; /* a(1) */
    prev[6] = base[0]; /* a(0) */

    for (idx_t i = 7; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-5) + a(i-7) */
        seq_t next = prev[0] + prev[4] + prev[6];

        /* shift buffer: prev[0] becomes a(i-1) for next iteration */
        for (int j = 6; j > 0; --j)
            prev[j] = prev[j - 1];
        prev[0] = next;
    }

    return prev[0]; /* now prev[0] holds a(n) */
}
