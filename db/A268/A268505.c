/*
 * A268505  a(n) = Product_{k=0..n} (4*k)!.
 * https://oeis.org/A268505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (4*k)!
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;            /* product so far */
    seq_t fact   = (seq_t)1;            /* current factorial (4*k)! */
    idx_t start = 1;                    /* next factor to multiply into fact */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {                    /* build (4*k)! from previous value */
            idx_t end = 4 * k;
            for (idx_t j = start; j <= end; ++j) {
                fact = seq_mul(fact, (seq_t)j);
            }
            start = end + 1;
        }
        result = seq_mul(result, fact);
    }
    return result;
}
