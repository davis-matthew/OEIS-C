/*
 * A353860  Number of collapsible integer compositions of n.
 * https://oeis.org/A353860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 53, u64 n <= 107,
 *            u128 n <= 203, big any n
 * Helpers:   compute_mu_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A353860: Number of collapsible integer compositions of n
   Formula: a(n) = 1 - ∑_{d|n, d>1} μ(d) * a(n/d)^d
   where μ is the Möbius function.
   Incremental computation with caching. 
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/compute_mu_upto.h"

/* Forward declaration of seq_t and idx_t – provided by the build system */
extern seq_t a(idx_t n);

/* Fast modular exponentiation for seq_t (binary exponentiation) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1U)
            result = result * base;
        exp >>= 1U;
        if (exp)
            base = base * base;
    }
    return result;
}

/* Cached data */
static seq_t *a_cache = NULL;          /* a[0..cache_limit] */
static size_t a_cap = 0;                /* allocated capacity of a_cache */
static idx_t cache_limit = 0;           /* last computed index */

static int8_t *mu_cache = NULL;        /* μ[0..mu_cap-1] */
static size_t mu_cap = 0;               /* allocated capacity of mu_cache */

/* Ensure cache covers up to n */
static void ensure_cache(idx_t n)
{
    size_t new_cap = (size_t)(n + 1);

    /* Allocate or grow a_cache */
    if (!a_cache) {
#ifdef __cplusplus
        a_cache = new seq_t[new_cap];
#else
        a_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
        a_cap = new_cap;
    } else if (new_cap > a_cap) {
        seq_t *old = a_cache;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < a_cap; ++i) tmp[i] = old[i];
        delete[] old;
        a_cache = tmp;
#else
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
#endif
        a_cap = new_cap;
    }

    /* Allocate or grow mu_cache */
    if (!mu_cache || new_cap > mu_cap) {
        if (!mu_cache)
            mu_cache = (int8_t *)malloc(new_cap * sizeof(int8_t));
        else
            mu_cache = (int8_t *)realloc(mu_cache, new_cap * sizeof(int8_t));
        mu_cap = new_cap;
    }

    /* Compute Möbius values up to n */
    compute_mu_upto(n, mu_cache);

    /* Ensure base values are set */
    if (n >= 0 && a_cache[0] == 0) a_cache[0] = (seq_t)0;
    if (n >= 1 && a_cache[1] == 0) a_cache[1] = (seq_t)1;

    /* Compute a(k) for all new indices */
    for (idx_t k = cache_limit + 1; k <= n; ++k) {
        if (k <= 1) continue;   /* a(0)=0, a(1)=1 already set */

        seq_t pos_sum = (seq_t)0;   /* Σ (μ=+1) terms */
        seq_t neg_sum = (seq_t)0;   /* Σ (μ=-1) terms */

        /* Enumerate divisors d>1, d<k */
        for (idx_t d = 2; d * d <= k; ++d) {
            if (k % d != 0) continue;

            int8_t mu_d = mu_cache[d];
            if (mu_d != 0) {
                seq_t term = pow_seq(a_cache[k / d], d);
                if (mu_d == 1) pos_sum += term;
                else           neg_sum += term;   /* mu_d == -1 */
            }

            idx_t d2 = k / d;
            if (d2 == d || d2 == 1) continue;   /* avoid double count, skip divisor 1 */

            int8_t mu_d2 = mu_cache[d2];
            if (mu_d2 != 0) {
                seq_t term = pow_seq(a_cache[k / d2], d2);
                if (mu_d2 == 1) pos_sum += term;
                else           neg_sum += term;   /* mu_d2 == -1 */
            }
        }

        /* Handle divisor d = k */
        int8_t mu_k = mu_cache[k];
        if (mu_k != 0) {
            if (mu_k == 1) pos_sum += (seq_t)1;   /* a(1)^k = 1 */
            else           neg_sum += (seq_t)1;   /* mu_k == -1 */
        }

        /* a(k) = (neg_sum + 1) - pos_sum */
        seq_t tmp = neg_sum + (seq_t)1;
        a_cache[k] = tmp - pos_sum;
    }

    cache_limit = n;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_cache(n);
    return a_cache[n];
}
