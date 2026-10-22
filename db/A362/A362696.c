/*
 * A362696  Expansion of e.g.f. Product_{k>0} (1 - x^(3*k-2))^(-1/(3*k-2)).
 * https://oeis.org/A362696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n≥1: a(n) = Σ_{k=1}^{n} A001817(k) * a(n-k) *
 *            P(n-1,k) where A001817(k) = number of divisors d of k such that
 *            d≡1 (mod 3), and P(m,k) = m!/(m-k+1)! (falling factorial)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */
#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n) {
    static idx_t max_computed = 0;          /* highest n for which a[] has been filled */
    static seq_t *a_vals = NULL;            /* array of a(n) values */
    static uint32_t *w_vals = NULL;         /* array of A001817(k) values */

    /* ensure storage for at least n+1 entries */
    if (n == 0) return (seq_t)1;

    /* Allocate arrays if they have not been allocated yet */
    if (a_vals == NULL) {
        size_t cap = n + 1;
        a_vals = (seq_t *)malloc(cap * sizeof(seq_t));
        w_vals = (uint32_t *)malloc(cap * sizeof(uint32_t));
        if (a_vals == NULL || w_vals == NULL) return 0;  /* allocation failure */
        a_vals[0] = (seq_t)1;
        w_vals[0] = 0;
    } else if ((size_t)(n + 1) > a_vals - a_vals) { /* enlarge if needed */
        size_t new_cap = n + 1;
        seq_t *new_a = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        uint32_t *new_w = (uint32_t *)realloc(w_vals, new_cap * sizeof(uint32_t));
        if (new_a == NULL || new_w == NULL) return 0;  /* allocation failure */
        a_vals = new_a;
        w_vals = new_w;
    }

    /* Compute w(k) for all k up to n if not already done */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        uint32_t count = 0;
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                if (d % 3 == 1) ++count;
                idx_t d2 = k / d;
                if (d2 != d && (d2 % 3 == 1)) ++count;
            }
        }
        w_vals[k] = count;
    }

    /* Compute a(n) sequentially up to n */
    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        seq_t prod = (seq_t)1;   /* P(cur-1, current k) */
        seq_t sum  = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t term = (seq_t)w_vals[k] * a_vals[cur - k] * prod;
            sum += term;
            if (k < cur) {
                prod *= (seq_t)(cur - k);
            }
        }
        a_vals[cur] = sum;
    }

    max_computed = n;
    return a_vals[n];
}
