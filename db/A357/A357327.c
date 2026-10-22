/*
 * A357327  a(n) is the unique nonnegative integer k <= A058084(n)/2 such that
 *          binomial(A058084(n),k) = n.
 * https://oeis.org/A357327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the unique n<=A058084(n)/2 with binomial(A058084(n),n)=n
 * Range:     u8 n <= 43, u16 n <= 9951, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;               /* convert n to seq_t for comparison */

    for (idx_t r = 1; ; ++r) {         /* iterate over rows */
        idx_t limit = r / 2;           /* consider k up to floor(r/2) */

        seq_t c = 1;                   /* binomial(r,0) */
        if (c == sn)                  /* special case n == 1 */
            return (seq_t)0;

        for (idx_t k = 1; k <= limit; ++k) {
            /* compute binomial(r,k) from binomial(r,k-1) */
            c = c * (seq_t)(r - k + 1) / (seq_t)k;

            if (c == sn)              /* found the correct k */
                return (seq_t)k;

            /* since the binomial values grow for this r, we can stop if we
               have already exceeded n (no later k in this row will match). */
            if (c > sn)
                break;
        }
    }
}
