/*
 * A344106  a(n) = n! * LaguerreL(n, -n+1).
 * https://oeis.org/A344106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} C(n,k)^2 * (n-k)! * (n-1)^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses only integer arithmetic and works for any
   build that provides seq_t and idx_t as described. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* External linkage as required. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* Allocate factorial and power arrays. */
    seq_t *fact = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *pow_n1 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!fact || !pow_n1) {
        /* Allocation failed – return 0 (undefined behaviour for the
           sequence is acceptable in this edge case). */
        return (seq_t)0;
    }

    /* Compute factorials up to n. */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* Compute powers of (n-1).  For n=1 the base is 0. */
    pow_n1[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        pow_n1[k] = pow_n1[k - 1] * (seq_t)(n - 1);
    }

    /* Sum the terms.  Compute binomial coefficients iteratively to
       avoid calls to the utility binomial function. */
    seq_t sum = (seq_t)0;
    seq_t bin = (seq_t)1;      /* C(n,0) = 1 */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin_sq = bin * bin;
        seq_t term = bin_sq * fact[n - k];
        term = term * pow_n1[k];
        sum = sum + term;

        if (k < n) {
            /* Update bin to C(n, k+1) = C(n, k) * (n-k)/(k+1). */
            bin = bin * (seq_t)(n - k);
            bin = bin / (seq_t)(k + 1);
        }
    }

    free(fact);
    free(pow_n1);
    return sum;
}
