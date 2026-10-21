/*
 * A250392  Number of length 6+3 0..n arrays with no four consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A250392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence: a(n) = 10*a(n-1) - 45*a(n-2) + 120*a(n-3) -
 *            210*a(n-4) + 252*a(n-5) - 210*a(n-6) + 120*a(n-7) - 45*a(n-8) +
 *            10*a(n-9) - a(n-10) for n > 10, with initial values a(1)=6,
 *            a(2)=966, …, a(10)=1015071750
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[11] = {
    0,                          /* unused 0th index */
    6,      966,      26578,   309452,   2160160,
    10755158, 42158796, 138336744, 395723154,
    1015071750
};

static seq_t *seq = NULL;
static idx_t seq_len = 0;   /* highest n computed; seq[1..seq_len] valid */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, return 0 */

    /* initialise cache with known values up to n=10 */
    if (seq == NULL) {
        seq = (seq_t *)malloc(11 * sizeof(seq_t));
        if (!seq) return 0;   /* out of memory, fallback to 0 */
        for (idx_t i = 1; i <= 10; ++i) seq[i] = init[i];
        seq_len = 10;
    }

    /* extend cache up to requested n */
    if (n > seq_len) {
        idx_t new_len = n + 1;            /* allocate up to index n */
        seq = (seq_t *)realloc(seq, new_len * sizeof(seq_t));
        if (!seq) return 0;              /* out of memory, fallback to 0 */

        for (idx_t i = seq_len + 1; i <= n; ++i) {
            seq[i] = 10 * seq[i-1]
                    - 45 * seq[i-2]
                    + 120 * seq[i-3]
                    - 210 * seq[i-4]
                    + 252 * seq[i-5]
                    - 210 * seq[i-6]
                    + 120 * seq[i-7]
                    - 45 * seq[i-8]
                    + 10 * seq[i-9]
                    - seq[i-10];
        }
        seq_len = n;
    }

    return seq[n];
}
