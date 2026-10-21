/*
 * A258667  A total of n married couples, including a mathematician M and his
 *          wife, are to be seated at the 2n chairs around a circular table,
 *          with no man seated next to his wife. After the ladies are seated
 *          at every other chair, M is the first man allowed to choose one of
 *          the remaining chairs. The sequence...
 * https://oeis.org/A258667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A258667
   Formula (OEIS, n > 5):
   a(n) = Σ_{k=0}^{n-1} (-1)^k (n-k-1)! Σ_{j=max(k-n+5,0)}^{min(k,4)}
          binomial(8-j, j) binomial(2n-k+j-10, k-j)
   a(n) = 0 for n ≤ 5
*/

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n ≤ 5 the definition fixes the value to 0. */
    if (n <= 5U)
        return 0;

    /* ---------- factorial cache ------------------------------------ */
    static seq_t *fact = NULL;   /* array of factorials, see fact[0] = 0! */
    static idx_t  fact_cap = 0;  /* largest index currently stored */

    /* We need factorials up to (n-1)! for the sum. */
    if (n > fact_cap + 1U) {
        /* Resize the cache to hold factorials 0!... (n-1)! */
        size_t new_cap = (size_t)n;                    /* number of entries */
        seq_t *new_fact;
        if (fact == NULL) {
            new_fact = (seq_t *)calloc(new_cap, sizeof(seq_t));
        } else {
            new_fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        }
        fact = new_fact;

        /* Initialise missing factorials. */
        if (fact_cap == 0U) fact[0] = 1;                 /* 0! = 1   */
        for (idx_t i = fact_cap + 1U; i < n; ++i) {
            fact[i] = fact[i - 1U] * (seq_t)i;
        }
        fact_cap = n - 1U;
    }

    /* ---------- main summation ------------------------------------- */
    seq_t pos_sum = 0;   /* Σ of terms with even k   */
    seq_t neg_sum = 0;   /* Σ of terms with odd  k   */

    for (idx_t k = 0U; k < n; ++k) {
        seq_t inner = 0;

        /* j runs from max(k-n+5,0) to min(k,4). For n>5, n-5 is safe. */
        idx_t j_min = (k >= n - 5U) ? (k - n + 5U) : 0U;
        idx_t j_max = (k < 4U) ? k : 4U;

        for (idx_t j = j_min; j <= j_max; ++j) {
            seq_t c1 = binomial(8U - j, j);
            idx_t  num = 2ULL * n - k + j - 10ULL;  /* second binomial's N   */
            idx_t  den = k - j;                     /* second binomial's K   */
            seq_t c2 = binomial(num, den);
            inner += c1 * c2;
        }

        seq_t term = fact[n - k - 1U] * inner;
        if ((k & 1U) == 0U)
            pos_sum += term;
        else
            neg_sum += term;
    }

    /* pos_sum ≥ neg_sum for all n > 5, so subtraction is safe. */
    return pos_sum - neg_sum;
}
