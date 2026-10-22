/*
 * A332918  Average number of binary strings of length n with Levenshtein
 *          distance <= 3 from a uniform randomly sampled binary string of
 *          this length, rounded to nearest integer.
 * https://oeis.org/A332918
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4) - 3*a(n-5) +
 *            3*a(n-6) - a(n-7) for n>16. Initial values (n: a(n)): 3: 8, 4:
 *            16, 5: 29, 6: 51, 7: 85, 8: 136, 9: 206, 10: 301, 11: 423, 12:
 *            577, 13: 768, 14: 998, 15: 1272, 16: 1594
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values up to n=16; index 0..16 */
    static const seq_t init[17] = {
        0, 0, 0,                 /* n=0,1,2 unused */
        8, 16, 29, 51, 85, 136, 206, /* n=3..9 */
        301, 423, 577, 768, 998, 1272, 1594  /* n=10..16 */
    };

    if (n <= 16) {
        /* n is guaranteed >=3 for correct use, but return 0 for <3 to be safe */
        if (n < 3) return (seq_t)0;
        return init[(size_t)n];
    }

    /* initialise last seven terms: a(10)..a(16) for n=17 */
    seq_t a0 = init[10]; /* a(n-7) */
    seq_t a1 = init[11];
    seq_t a2 = init[12];
    seq_t a3 = init[13];
    seq_t a4 = init[14];
    seq_t a5 = init[15];
    seq_t a6 = init[16]; /* a(n-1), current value for n=17-1 */

    for (idx_t i = 17; i <= n; ++i) {
        seq_t next = 3 * a6;
        next -= 3 * a5;
        next += a4;
        next += a3;
        next -= 3 * a2;
        next += 3 * a1;
        next -= a0;

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }

    return a6;
}
