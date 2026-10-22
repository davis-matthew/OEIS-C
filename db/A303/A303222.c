/*
 * A303222  Total volume of all rectangular prisms with dimensions p, q and (p
 *          + q)/2 such that p and q are squarefree, n = p + q and p <= q.
 * https://oeis.org/A303222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/2) * Sum_{i=1..floor(n/2)} i * (n-i) * mu(i)^2 *
 *            mu(n-i)^2 where mu(i)^2 = 1 iff i is squarefree; otherwise 0
 * Range:     u8 n <= 11, u16 n <= 43, u32 n <= 674, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t half = n / 2;
    for (idx_t i = 1; i <= half; ++i) {
        if (is_squarefree((seq_t)i) && is_squarefree((seq_t)(n - i))) {
            seq_t prod = (seq_t)i * (seq_t)(n - i);
            sum += prod;
        }
    }
    return ((seq_t)n * sum) / 2;
}
