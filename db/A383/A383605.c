/*
 * A383605  Expansion of 1/( (1-x) * (1-x-9*x^2)^2 )^(1/3).
 * https://oeis.org/A383605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊n/2⌋} 3^k * Π_{j=1}^{k} (3*j–1) / k! *
 *            binomial(n–k,k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A383605, offset 0 */

#include <stddef.h>
#include "../utils/binomial.h"

static seq_t *T_cache = NULL;      /* T_k = 3^k * Π_{j=1}^k (3*j-1) / k!  */
static size_t T_len  = 0;          /* number of cached values so far   */

/* Ensure that T_cache has valid entries for k = 0 .. maxk */
static void ensure_T(idx_t maxk)
{
    size_t new_cap = (size_t)maxk + 1;
    if (new_cap <= T_len)
        return;

    /* grow array */
    T_cache = (seq_t *)realloc(T_cache, new_cap * sizeof(seq_t));
    /* initialise new slots to 0 (will be overwritten below) */
    for (size_t i = T_len; i < new_cap; ++i)
        T_cache[i] = (seq_t)0;

    /* initialise base case if starting from scratch */
    if (T_len == 0) {
        T_cache[0] = (seq_t)1;   /* T_0 = 1 */
        T_len = 1;
    }

    /* compute missing T_k values */
    for (size_t k = T_len; k < new_cap; ++k) {
        /* k ≥ 1 */
        seq_t prev = T_cache[k - 1];
        seq_t mult = (seq_t)(3 * (3 * (idx_t)k - 1)); /* 3*(3k-1) */
        seq_t num   = prev * mult;
        T_cache[k] = num / (seq_t)k;    /* division is exact by construction */
    }

    T_len = new_cap;
}

seq_t a(idx_t n)
{
    idx_t maxk = n / 2;
    ensure_T(maxk);

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = T_cache[k] * binomial(n - k, k);
        sum += term;
    }
    return sum;
}
