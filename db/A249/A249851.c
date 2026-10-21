/*
 * A249851  Number of length 7+4 0..n arrays with no five consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A249851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 66*a(n-2) + 220*a(n-3) - 495*a(n-4) +
 *            792*a(n-5) * - 924*a(n-6) + 792*a(n-7) - 495*a(n-8) + 220*a(n-9)
 *            - 66*a(n-10) * + 12*a(n-11) - a(n-12) for n > 12 * Initial
 *            values (offset 1): a(1)=10, a(2)=3966, a(3)=223505,
 *            a(4)=4527441, * a(5)=49270055, a(6)=354521087,
 *            a(7)=1901506446,...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes A249851 */
seq_t a(idx_t n)
{
    /* return 0 for non‑positive indices, behaviour below offset is arbitrary */
    if (n == 0) return 0;

    /* hard‑coded initial terms */
    static const seq_t init[13] = {
        0ULL,                /* dummy to make index 1-based */
        10ULL,
        3966ULL,
        223505ULL,
        4527441ULL,
        49270055ULL,
        354521087ULL,
        1901506446ULL,
        8194431990ULL,
        29822392392ULL,
        94893269020ULL,
        270701817639ULL,
        705393878111ULL
    };

    if (n <= 12) return init[n];

    /* sliding window of the last 12 values */
    seq_t h[12];
    for (idx_t i = 0; i < 12; ++i) h[i] = init[i + 1];

    for (idx_t cur = 13; cur <= n; ++cur) {
        /* recurrence: a(cur) = 12*a(cur-1) - 66*a(cur-2) + ... - a(cur-12) */
        seq_t val = 12ULL * h[11]
                    - 66ULL * h[10]
                    + 220ULL * h[9]
                    - 495ULL * h[8]
                    + 792ULL * h[7]
                    - 924ULL * h[6]
                    + 792ULL * h[5]
                    - 495ULL * h[4]
                    + 220ULL * h[3]
                    - 66ULL * h[2]
                    + 12ULL * h[1]
                    - h[0];

        /* shift the window */
        for (int k = 0; k < 11; ++k) h[k] = h[k + 1];
        h[11] = val;
    }

    return h[11];
}
