/*
 * A256746  Number of (n+2)X(6+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 3 and no antidiagonal sum 3 and no row sum 0 and no column sum
 *          0.
 * https://oeis.org/A256746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2*a(n-1) + 4*a(n-2) + 24*a(n-3) + 27*a(n-4) - 70*a(n-5)
 *            - 198*a(n-6) - 88*a(n-7) + 408*a(n-8) + 648*a(n-9) - 48*a(n-10)
 *            - 896*a(n-11) - 704*a(n-12) + 384*a(n-13) + 512*a(n-14) for n >
 *            18 Initial values: a(1)=12914 , a(2)=12280 , a(3)=16560 ,
 *            a(4)=18688 , a(5)=47440 , a(6)=72192 , a(7)=155...
 * Range:     u8 none, u16 n <= 5, u32 n <= 22, u64 n <= 54, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static const seq_t base[19] = {
    0,                                 /* unused base[0] */
    (seq_t)12914,   (seq_t)12280,      (seq_t)16560,   (seq_t)18688,
    (seq_t)47440,   (seq_t)72192,      (seq_t)155792,  (seq_t)255488,
    (seq_t)600080,  (seq_t)869632,     (seq_t)2236176, (seq_t)3351808,
    (seq_t)8175376, (seq_t)12600576,   (seq_t)31831312,(seq_t)46718208,
    (seq_t)122490640,(seq_t)182095104   /* up to base[18] */
};

static const int coeff[14] = {
    -2,  4,  24, 27, -70, -198, -88, 408, 648, -48, -896, -704, 384, 512
};

seq_t a(idx_t n)
{
    /* Return precomputed values for the first 18 terms. */
    if (n <= 18) {
        return base[n];
    }

    /* For n > 18 use the empirical recurrence. */
    seq_t last[14];               /* last[i] holds a(n-1-i) */
    for (int i = 0; i < 14; ++i) {
        last[i] = base[18 - i];
    }

    seq_t result = 0;
    for (idx_t step = 19; step <= n; ++step) {
        seq_t pos = 0, neg = 0;
        for (int i = 0; i < 14; ++i) {
            int c = coeff[i];
            if (c >= 0) {
                pos += last[i] * (seq_t)c;
            } else {
                neg += last[i] * (seq_t)(-c);
            }
        }
        result = pos - neg;    /* pos >= neg because of the recurrence */

        /* shift the last array: discard the oldest, prepend new */
        for (int i = 13; i > 0; --i) {
            last[i] = last[i - 1];
        }
        last[0] = result;
    }
    return result;
}
