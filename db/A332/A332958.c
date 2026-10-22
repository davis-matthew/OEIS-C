/*
 * A332958  Number of labeled forests with 2n nodes consisting of n-1 isolated
 *          nodes and a labeled tree with n+1 nodes.
 * https://oeis.org/A332958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2*n,n-1) * (n+1)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    /* offset: a(1) is the first term */
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */
    idx_t k = n - 1;                /* exponent and binomial lower index */
    seq_t bin = binomial(2 * n, k);
    seq_t pow = big_pow((seq_t)(n + 1), (seq_t)k);
    return seq_mul(bin, pow);
}
