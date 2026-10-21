/*
 * A246891  Number of length n+4 0..7 arrays with some pair in every
 *          consecutive five terms totalling exactly 7.
 * https://oeis.org/A246891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) +10*a(n-2) +38*a(n-3) +132*a(n-4) -233*a(n-5)
 *            -124*a(n-6) -48*a(n-7) -152*a(n-8) +232*a(n-9) +97*a(n-10)
 *            -80*a(n-11) +14*a(n-12) -6*a(n-13) -4*a(n-14) + a(n-15) (n ≥ 16)
 *            Initial values (n=1..15): 23400, 159688, 1079680, 7216512,
 *            47799488, 319477960, 2132283592, 14219886016, 948265...
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 19, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial 15 terms */
    static const seq_t init[15] = {
        23400ULL,            /* a(1) */
        159688ULL,           /* a(2) */
        1079680ULL,          /* a(3) */
        7216512ULL,          /* a(4) */
        47799488ULL,         /* a(5) */
        319477960ULL,        /* a(6) */
        2132283592ULL,       /* a(7) */
        14219886016ULL,      /* a(8) */
        94826520320ULL,      /* a(9) */
        632600369216ULL,     /* a(10) */
        4219646774184ULL,    /* a(11) */
        28145459043464ULL,   /* a(12) */
        187735748042496ULL,  /* a(13) */
        1252250358585088ULL, /* a(14) */
        8352796888611456ULL  /* a(15) */
    };

    if (n <= 15) {
        return init[n - 1];
    }

    /* Coefficients for the recurrence, ordered as a(n-1) ... a(n-15) */
    static const int64_t coeff[15] = {
        4, 10, 38, 132, -233, -124, -48, -152,
        232, 97, -80, 14, -6, -4, 1
    };

    /* Buffer holding the last 15 values:
       buf[0] = a(k-1), buf[1] = a(k-2), ..., buf[14] = a(k-15)  */
    seq_t buf[15];
    for (int i = 0; i < 15; ++i) {
        buf[i] = init[14 - i]; /* init[14] = a(15), init[13] = a(14), ... */
    }

    seq_t value;
    for (idx_t k = 16; k <= n; ++k) {
        seq_t sum = 0;
        for (int i = 0; i < 15; ++i) {
            int64_t c = coeff[i];
            seq_t term = buf[i] * ((seq_t)(c >= 0 ? c : -c));
            if (c < 0) {
                sum -= term;
            } else {
                sum += term;
            }
        }
        /* shift buffer */
        for (int i = 14; i > 0; --i) {
            buf[i] = buf[i - 1];
        }
        buf[0] = sum;
        value = sum;
    }

    return value;
}
