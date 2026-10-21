/*
 * A297938  Number of nX3 0..1 arrays with every element equal to 0, 2, 3 or 5
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A297938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - a(n-2) - 7*a(n-3) - 5*a(n-4) + 11*a(n-5)
   + 22*a(n-6) - 15*a(n-7) - 15*a(n-8) - 22*a(n-9) - 25*a(n-10)
   + 55*a(n-11) + 35*a(n-12) - 15*a(n-13) - 6*a(n-14) - 11*a(n-15) - 4*a(n-16)
   for n > 17.
   Initial values:
   a(1)=1, a(2)=7, a(3)=4, a(4)=14, a(5)=16, a(6)=51, a(7)=102,
   a(8)=258, a(9)=667, a(10)=1716, a(11)=4557, a(12)=12280,
   a(13)=33101, a(14)=90062, a(15)=245804, a(16)=671771,
   a(17)=1839762 */
#include <stddef.h>
#include <stdint.h>

static const seq_t init_vals[17] = {
    (seq_t)1,      (seq_t)7,      (seq_t)4,      (seq_t)14,
    (seq_t)16,     (seq_t)51,     (seq_t)102,    (seq_t)258,
    (seq_t)667,    (seq_t)1716,   (seq_t)4557,   (seq_t)12280,
    (seq_t)33101,  (seq_t)90062,  (seq_t)245804, (seq_t)671771,
    (seq_t)1839762
};

static const int coeff[16] = {
    4,   -1,  -7,  -5,  11,
    22,  -15, -15, -22, -25,
    55,   35,  -15,  -6, -11, -4
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */
    if (n <= 17) return init_vals[n - 1];

    /* Buffer to hold the previous 16 terms:
       last[0] = a(i-1), last[1] = a(i-2), ..., last[15] = a(i-16) */
    seq_t last[16];
    for (int j = 0; j < 16; ++j)
        last[j] = init_vals[16 - j];   /* last[0]=a(17), last[15]=a(2) */

    seq_t result = 0;
    for (idx_t i = 18; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 0; k < 16; ++k) {
            seq_t term = last[k];
            int c = coeff[k];
            if (c > 0)        pos += (seq_t)c * term;
            else if (c < 0)   neg += (seq_t)(-c) * term;
        }
        result = pos - neg;          /* pos >= neg for all n */

        /* Shift buffer: drop a(i-16), push new a(i) */
        for (int j = 15; j > 0; --j) last[j] = last[j - 1];
        last[0] = result;
    }
    return result;
}
