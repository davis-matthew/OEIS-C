/*
 * A285853  Number of permutations of [n] with two ordered cycles such that
 *          equal-sized cycles are ordered with increasing least elements.
 * https://oeis.org/A285853
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{⌊(n-1)/2⌋} 2 * C(n,k) * (k-1)! * (n-k-1)! * +
 *            (n even ? (C(n,n/2) * ((n/2)-1)!^2) / 2 : 0) (OEIS A285853) * *
 *            Here C(n,k) is the binomial coefficient. The function computes
 *            the * required sum by building factorials up to (n-2)! and
 *            evaluating * each term exactly using only integ...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2u) return (seq_t)0;   /* behaviour below offset is irrelevant */

    /* Pre‑compute factorials up to (n-2)! */
    idx_t max_fact = (n > 1u) ? (n - 2u) : 0u;
    seq_t *fact = (seq_t *)malloc((size_t)(max_fact + 1u) * sizeof(seq_t));
    if (!fact) return (seq_t)0;    /* allocation failure – unlikely in tests */

    fact[0] = (seq_t)1;
    for (idx_t i = 1u; i <= max_fact; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    seq_t result = (seq_t)0;

    /* Sum over k = 1 .. floor((n-1)/2) with weight 2 */
    idx_t limit = (n - 1u) / 2u;
    for (idx_t k = 1u; k <= limit; ++k) {
        /* Compute binomial coefficient C(n,k) */
        seq_t bin = (seq_t)1;
        for (idx_t i = 1u; i <= k; ++i)
            bin = bin * (seq_t)(n - k + i) / (seq_t)i;

        seq_t term = bin * fact[k - 1u] * fact[n - k - 1u];
        result += term * (seq_t)2;
    }

    /* Add the special even‑size case if n is even */
    if ((n & 1u) == 0u) {
        idx_t k = n / 2u;
        seq_t bin = (seq_t)1;
        for (idx_t i = 1u; i <= k; ++i)
            bin = bin * (seq_t)(n - k + i) / (seq_t)i;

        seq_t term = bin * fact[k - 1u] * fact[k - 1u];
        result += term / (seq_t)2;
    }

    free(fact);
    return result;
}
