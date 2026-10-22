/*
 * A360802  Expansion of Sum_{k>0} (x / (1 - (2 * x)^k))^k.
 * https://oeis.org/A360802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} 2^(n-d) * binomial(d + n/d - 2, d-1)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binom, int_sqrt_simple, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"
#include "../utils/int_sqrt_simple.h"

static seq_t term_for_divisor(idx_t d, idx_t n) {
    /* Computes 2^(n-d) * binomial(d + n/d - 2, d-1) for one divisor d */
    idx_t exp = n - d;
    seq_t pow2_val = pow2(exp);                           /* 2^(n-d) */
    idx_t b_n = d + n / d - 2;                            /* d + n/d - 2 */
    idx_t b_k = d - 1;                                    /* d-1 */
    seq_t bin_val = binom(b_n, b_k);                      /* binomial(b_n, b_k) */
    return pow2_val * bin_val;                            /* multiply */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                                 /* undefined for n=0 */
    seq_t sum = 0;
    idx_t root = int_sqrt_simple(n);

    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;                              /* paired divisor */
            sum += term_for_divisor(d, n);
            if (e != d)                                   /* avoid double count */
                sum += term_for_divisor(e, n);
        }
    }
    return sum;
}
