/*
 * A298391  Number of nX3 0..1 arrays with every element equal to 1, 2, 4, 5
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -3*a(n-2) + a(n-3) -11*a(n-4) +12*a(n-5)
 *            -4*a(n-6) * +19*a(n-8) -20*a(n-9) +29*a(n-10) -65*a(n-11)
 *            +63*a(n-12) * -34*a(n-13) +10*a(n-15) for n > 17 * * Initial
 *            values for 1 ≤ n ≤ 17 are taken from the OEIS table. The *
 *            recurrence is used for larger n. The algorithm uses only
 *            O(17)...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Sequence values for 1 ≤ n ≤ 17.  The 0‑th element is unused. */
static const seq_t init_seq[18] = {
    0ULL,                    /* 0 – dummy, not used */
    1ULL, 7ULL, 15ULL, 29ULL,
    63ULL, 167ULL, 473ULL, 1205ULL,
    3161ULL, 8399ULL, 22021ULL,
    58279ULL, 153851ULL, 405489ULL,
    1070981ULL, 2826361ULL, 7458763ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, return 0 */

    /* For n up to 17, return the pre‑computed value. */
    if (n <= 17) return init_seq[(idx_t)n];

    /* Circular buffer holding the last 17 computed values:
     * win[0]  = a[i-16]   (oldest)
     * win[15] = a[i-1]
     * win[16] = a[i]      (new value)
     */
    seq_t win[17];
    for (int i = 0; i < 17; ++i) win[i] = init_seq[i+1];

    /* Compute terms for i = 18 … n */
    for (idx_t i = 18; i <= n; ++i) {
        /* Positive contributions */
        seq_t pos = 4 * win[16]          /* 4*a[i-1] */
                 + win[14]                /* +a[i-3] */
                 + 12 * win[12]           /* +12*a[i-5] */
                 + 19 * win[9]            /* +19*a[i-8] */
                 + 29 * win[7]            /* +29*a[i-10] */
                 + 63 * win[5]            /* +63*a[i-12] */
                 + 10 * win[2];           /* +10*a[i-15] */

        /* Negative contributions */
        seq_t neg = 3 * win[15]          /*  -3*a[i-2] */
                 + 11 * win[13]          /*  -11*a[i-4] */
                 + 4 * win[11]           /*   -4*a[i-6] */
                 + 20 * win[8]           /*  -20*a[i-9] */
                 + 65 * win[6]           /*  -65*a[i-11] */
                 + 34 * win[4];          /*  -34*a[i-13] */

        seq_t val = pos - neg;           /* a[i] = pos - neg */

        /* Shift the window: drop the oldest value and insert the new one */
        for (int j = 0; j < 16; ++j) win[j] = win[j+1];
        win[16] = val;
    }

    /* The last computed value is now in win[16] */
    return win[16];
}
