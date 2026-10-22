/*
 * A372372  Coefficient of x^n in the expansion of ( (1+x+x^3)^3 / (1+x) )^n.
 * https://oeis.org/A372372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊n/3⌋} binomial(3n, k) * binomial(2n – k, n –
 *            3k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses binomial() and seq_mul() helpers from the utils
 * library.  The implementation is O(n) per call. */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    seq_t total = 0;
    idx_t maxk = n / 3;               /* floor division */
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t three_n = n + n + n;           /* 3*n avoiding overflow */
        idx_t two_n   = n + n;               /* 2*n */
        seq_t term1   = binomial(three_n, k);
        seq_t term2   = binomial(two_n - k, n - 3 * k);
        term1 = seq_mul(term1, term2);
        total += term1;
    }
    return total;
}
