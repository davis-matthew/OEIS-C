/*
 * A251038  Number of (1+1)X(n+1) 0..3 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A251038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 25*a(n-2) - 30*a(n-3) + 146*a(n-4) +
 *            31*a(n-5) * - 287*a(n-6) - 50*a(n-7) + 239*a(n-8) + 59*a(n-9) -
 *            86*a(n-10) * - 27*a(n-11) + 11*a(n-12) + 4*a(n-13). * Initial
 *            values: a(1)=110, a(2)=711, a(3)=4050, a(4)=21720, a(5)=110852,
 *            * a(6)=546600, a(7)=2624313, a(8)=12343573, a(9)=57...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int coeffs[13] = {10, -25, -30, 146, 31, -287, -50, 239, 59,
                               -86, -27, 11, 4};

static const seq_t initial[13] = {
    110,          /* a(1) */
    711,          /* a(2) */
    4050,         /* a(3) */
    21720,        /* a(4) */
    110852,       /* a(5) */
    546600,       /* a(6) */
    2624313,      /* a(7) */
    12343573,     /* a(8) */
    57116869,     /* a(9) */
    260828863,    /* a(10) */
    1178253507,   /* a(11) */
    5274772080,   /* a(12) */
    23435131926   /* a(13) */
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* not defined for n=0 */
    if (n <= 13) return initial[n-1];

    /* Allocate array for values up to n (index 0 unused). */
    seq_t *buf = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!buf) return 0; /* allocation failure */

    for (idx_t i = 1; i <= 13; ++i)
        buf[i] = initial[i - 1];

    for (idx_t i = 14; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (int k = 0; k < 13; ++k) {
            seq_t term = buf[i - (k + 1)];
            int c = coeffs[k];
            if (c >= 0) {
                pos += (seq_t)c * term;
            } else {
                neg += (seq_t)(-c) * term;
            }
        }
        buf[i] = pos - neg; /* guaranteed non‑negative */
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
