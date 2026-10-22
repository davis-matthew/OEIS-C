/*
 * A360195  Number of acyclic spanning subgraphs in the 4 X n grid graph.
 * https://oeis.org/A360195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   mul_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function for A360195.
   For n ≥ 1:
     a(n) = p_n − Σ_{k=1}^{min(10,n)} q_k · a(n−k)
   where
     p(x) = x · (8 − 332x + 4922x² − 34224x³ + 120160x⁴ − 215504x⁵
                  + 196576x⁶ − 88512x⁷ + 17408x⁸ − 1024x⁹)
     Q(x) = 1 − 146x + 5145x² − 74504x³ + 525120x⁴ − 1885344x⁵
            + 3430128x⁶ − 3147392x⁷ + 1418496x⁸ − 278528x⁹ + 16384x¹⁰
   Thus
     p_n = the coefficient of xⁿ in P(x) (zero for n>10)
     q_k = the k‑th coefficient in Q(x) (with sign included)
   a(0) is defined as 0, a(1)=8, a(2)=836, ... as given in OEIS.
   The function below memoises values so that calls in any order
   remain O(1) after the first computation.
*/

#include <stddef.h>     /* size_t */
#include <stdlib.h>     /* abort */
#include <stdbool.h>
#include "../utils/mul_small.h"      /* seq_t mul_small(seq_t, unsigned) */
#include "../utils/seq_realloc.h"    /* seq_t *seq_realloc(seq_t*,size_t,size_t) */

static seq_t  *cache  = NULL;   /* cache[0] .. cache[n] stores a(0) .. a(n)   */
static size_t cache_len = 0;    /* number of elements stored (== cache_cap) */
static size_t cache_cap = 0;    /* current capacity of cache */

/* Grow the cache to hold at least `need` elements (index up to need-1). */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_ptr = seq_realloc(cache, cache_cap, new_cap);
    if (!new_ptr) abort();
    cache     = new_ptr;
    cache_cap = new_cap;
}

/* Coefficients of numerator P(x) and denominator Q(x). */
static const long long num_c[10] = {
     8LL,  -332LL,   4922LL,  -34224LL,  120160LL,
 -215504LL,  196576LL,  -88512LL,   17408LL,   -1024LL
};

static const long long den_c[10] = {
 -146LL,  5145LL, -74504LL, 525120LL, -1885344LL,
 3430128LL, -3147392LL, 1418496LL, -278528LL, 16384LL
};

/* Initialise cache up to index `upto` (inclusive).  If `upto` < 0 nothing
   is done; otherwise the cache will have entries 0..upto. */
static void init_cache(size_t upto)
{
    if (upto + 1 <= cache_len) return;          /* already computed */
    size_t needed = upto + 1;
    ensure_capacity(needed);

    /* Compute values from current cache_len up to upto. */
    for (size_t idx = cache_len; idx < needed; ++idx) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;

        if (idx == 0) {                                  /* a(0) = 0 */
            cache[idx] = 0;
            continue;
        }

        /* Handle numerator term p_n. */
        if (idx <= 10) {
            long long pk = num_c[idx - 1];
            if (pk >= 0)
                pos_sum += (seq_t)pk;
            else
                neg_sum += (seq_t)(-pk);
        }

        /* Sum contributions of previous a(k) with coefficients -q_k. */
        size_t limit = (idx <= 10) ? (idx - 1) : 10;
        for (size_t k = 1; k <= limit; ++k) {
            long long coeff = den_c[k - 1];          /* q_k */
            seq_t term = cache[idx - k];
            if (coeff >= 0)
                /* subtract q_k * term */
                neg_sum += mul_small(term, (unsigned)coeff);
            else
                /* subtract negative coeff -> add abs(coeff) * term */
                pos_sum += mul_small(term, (unsigned)(-coeff));
        }

        cache[idx] = pos_sum - neg_sum;   /* pos_sum ≥ neg_sum for this seq. */
    }
    cache_len = needed;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    size_t idx = (size_t)n;
    init_cache(idx);                      /* ensure cache[0..idx] exist */
    return cache[idx];
}
