/*
 * A362322  a(n) = n! * Sum_{k=0..floor(n/4)} (-n)^k / (k! * (n-4*k)!).
 * https://oeis.org/A362322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/4)} (-n)^k / (k! * (n-4*k)!)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 17, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* maximum k such that 4*k <= n */
    int max_k = (int)(n / 4);
    seq_t sum = 0;
    seq_t t   = 1;          /* term t_0 = 1 */

    for (int k = 0; k <= max_k; ++k) {
        sum += t;           /* add current term to the sum */

        if (k == max_k) break;   /* no next term */

        /* compute product of the four integers
           (n-4*(k+1)+1) ... (n-4*(k+1)+4) */
        seq_t prod = 1;
        for (int i = 1; i <= 4; ++i) {
            idx_t val = n - (idx_t)4 * (k + 1) + (idx_t)i;
            prod *= (seq_t)val;
        }

        /* t_{k+1} = t_k * (-n) * prod / (k+1)  */
        t = t * (seq_t)(- (int64_t)n) * prod / (seq_t)(k + 1);
    }

    return sum;
}
