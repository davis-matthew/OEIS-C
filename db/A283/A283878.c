/*
 * A283878  An eventually quasilinear solution to Hofstadter's Q recurrence.
 * https://oeis.org/A283878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=2; for n≥3: a(3k)=3k, a(3k+1)=1, a(3k+2)=3; For
 *            n>8: a(n)=2*a(n-3)-a(n-6)
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(idx_t n) returns the n-th term of OEIS A283878. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Maintain the last six values in a circular buffer. */
    seq_t vals[6] = {0};
    idx_t i;
    for (i = 1; i <= n; ++i)
    {
        seq_t val;
        if (i <= 8)           /* directly from the known initial pattern */
        {
            if (i == 1) {
                val = 0;
            } else if (i == 2) {
                val = 2;
            } else if (i % 3 == 0) {
                val = (seq_t)i;
            } else if (i % 3 == 1) {
                val = 1;
            } else {            /* i % 3 == 2 */
                val = 3;
            }
        } else                /* recurrence for n > 8 */
        {
            val = 2 * vals[(i - 3) % 6] - vals[(i - 6) % 6];
        }
        vals[i % 6] = val;
    }
    return vals[n % 6];
}
