/*
 * A251087  Number of (n+1) X (7+1) 0..2 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 13*a(n-2) + 10*a(n-3) + 5*a(n-4) - 14*a(n-5) +
 *            9*a(n-6) - 2*a(n-7), n > 8 Initial values (offset 1): a(1)=3593,
 *            a(2)=9086, a(3)=15411, a(4)=28616, a(5)=48017, a(6)=82898,
 *            a(7)=139553, a(8)=240972
 * Range:     u8 none, u16 n <= 5, u32 n <= 22, u64 n <= 54, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial terms indexed from 1 to 8 */
    static const seq_t init[9] = {
        (seq_t)0,          /* unused 0‑th entry */
        (seq_t)3593,
        (seq_t)9086,
        (seq_t)15411,
        (seq_t)28616,
        (seq_t)48017,
        (seq_t)82898,
        (seq_t)139553,
        (seq_t)240972
    };

    if (n <= (idx_t)8)
        return init[(size_t)n];

    /* Keep the last 8 computed values: prev[0] = a(k-7), … , prev[7] = a(k-1) */
    seq_t prev[8];
    for (size_t i = 0; i < 8; ++i)
        prev[i] = init[i + 1];

    for (idx_t k = (idx_t)9; k <= n; ++k) {
        seq_t val  = ((seq_t)6  * prev[7])   /* a(k-1) */
                  - ((seq_t)13 * prev[6])   /* a(k-2) */
                  + ((seq_t)10 * prev[5])   /* a(k-3) */
                  + ((seq_t)5  * prev[4])   /* a(k-4) */
                  - ((seq_t)14 * prev[3])   /* a(k-5) */
                  + ((seq_t)9  * prev[2])   /* a(k-6) */
                  - ((seq_t)2  * prev[1]);  /* a(k-7) */

        /* Shift window: discard a(k-7), push a(k) at the end */
        for (size_t i = 0; i < 7; ++i)
            prev[i] = prev[i + 1];
        prev[7] = val;
    }

    return prev[7];  /* corresponds to a(n) */
}
