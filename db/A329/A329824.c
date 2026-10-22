/*
 * A329824  Number of length-n binary words having no palindromes of length >
 *          5 as contiguous subwords.
 * https://oeis.org/A329824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-6) + 2*a(n-7) + 2*a(n-8) + 2*a(n-9) + a(n-10) for n
 *            >= 20 Initial terms: a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=32,
 *            a(6)=56, a(7)=84, a(8)=120, a(9)=164, a(10)=226, a(11)=312,
 *            a(12)=428, a(13)=584, a(14)=802, a(15)=1096, a(16)=1500,
 *            a(17)=2056, a(18)=2816, a(19)=3852,
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 63, u64 n <= 133,
 *            u128 n <= 275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed initial values.  For n <= 19 we return directly. */
    static const seq_t init[19] = {
        2ULL, 4ULL, 8ULL, 16ULL, 32ULL, 56ULL, 84ULL, 120ULL, 164ULL,
        226ULL, 312ULL, 428ULL, 584ULL, 802ULL, 1096ULL, 1500ULL, 2056ULL, 2816ULL, 3852ULL
    };

    if (n <= 19)
        return init[n - 1];

    /* Circular buffer holding the last 10 computed values:
       last[0] = a(i-10), last[1] = a(i-9), ..., last[9] = a(i-1)   */
    seq_t last[10] = {
        init[9], init[10], init[11], init[12], init[13],
        init[14], init[15], init[16], init[17], init[18]
    };

    seq_t cur = 0;

    for (idx_t i = 20; i <= n; ++i) {
        /* Apply the recurrence relation. */
        cur = 3 * last[4] + 2 * last[3] + 2 * last[2] + 2 * last[1] + last[0];

        /* Shift the window: discard the oldest value and append new one. */
        last[0] = last[1];
        last[1] = last[2];
        last[2] = last[3];
        last[3] = last[4];
        last[4] = last[5];
        last[5] = last[6];
        last[6] = last[7];
        last[7] = last[8];
        last[8] = last[9];
        last[9] = cur;
    }

    return cur;
}
