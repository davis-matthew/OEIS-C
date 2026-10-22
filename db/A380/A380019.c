/*
 * A380019  Expansion of e.g.f. 1/(1 - 4*x*exp(x))^(1/4).
 * https://oeis.org/A380019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k) * k^(n-k) * Π_{i=0}^{k-1}
 *            (4*i+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the identity binomial(-1/4,k) = (-1)^k * Π(4i+1)/(4^k*k!) to cancel signs
   and powers of 4, leaving only positive integers. */

#include <stdlib.h>
#include "../utils/big_pow.h"

static seq_t *prod = NULL;     /* prod[k] = ∏_{i=0}^{k-1} (4*i+1) , prod[0] = 1 */
static size_t prod_cap = 0;

/* Ensure the prod array is allocated up to index n (inclusive). */
static void
ensure_prod(idx_t n)
{
    if (n + 1 <= prod_cap)
        return;

    size_t old_cap = prod_cap;
    size_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(prod, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    prod = tmp;

    if (old_cap == 0) {
        prod[0] = (seq_t)1;
        old_cap = 1;
    }

    for (size_t i = old_cap; i < new_cap; ++i) {
        /* prod[i] = prod[i-1] * (4*(i-1)+1) */
        prod[i] = prod[i-1] * (seq_t)(4 * (i - 1) + 1);
    }
    prod_cap = new_cap;
}

seq_t
a(idx_t n)
{
    ensure_prod(n);

    seq_t result = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute binomial coefficient C(n,k) multiplicatively. */
        idx_t k_small = (k <= (n - k)) ? k : (n - k);
        seq_t bin = (seq_t)1;
        for (idx_t i = 1; i <= k_small; ++i) {
            bin = bin * (seq_t)(n - k_small + i) / (seq_t)i;
        }

        /* Compute k^(n-k), handling the 0^0 case. */
        seq_t powk;
        if (k == 0) {
            powk = (n == 0) ? (seq_t)1 : (seq_t)0;
        } else {
            powk = big_pow((seq_t)k, (seq_t)(n - k));
        }

        /* Retrieve prod_k from the pre‑computed table. */
        seq_t prod_k = prod[k];

        /* Term = C(n,k) * k^(n-k) * prod_k. */
        seq_t term = bin * powk;
        term = term * prod_k;

        result += term;
    }

    return result;
}
