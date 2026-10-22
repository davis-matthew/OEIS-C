/*
 * A362713  Expansion of e.g.f. x*2F1([3/4, 3/4], [3/2], 4*x^2)/2F1([1/4,
 *          1/4], [1/2], 4*x^2), odd powers only.
 * https://oeis.org/A362713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{j=1..n} (4*j-1)^2 * - Sum_{m=0..n-1}
 *            binomial(2*n+1, 2*m+1) * * Product_{j=1..n-m} (4*j-3)^2 * * a(m)
 *            * Initial value: a(0)=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"          /* provides seq_t binomial(idx_t a, idx_t b) */

static seq_t *a_cache = NULL;        /* a[0..a_len-1]   */
static idx_t   a_len   = 0;          /* number of stored terms   */

static seq_t *prod1_cache = NULL;    /* Π(4*j-1)^2 for j=1..i , prod1_cache[0]=1   */
static seq_t *prod2_cache = NULL;    /* Π(4*j-3)^2 for j=1..i , prod2_cache[0]=1   */
static idx_t   prod_len = 0;         /* current allocated length   */

/* Ensure product caches contain values up to index n (inclusive). */
static void ensure_prod(idx_t n)
{
    if (prod_len > n)
        return;

    idx_t old_len = prod_len;                /* number of elements currently stored */
    size_t new_cap = (size_t)(n + 1);

    /* Allocate new arrays or grow existing ones */
    seq_t *new1 = (seq_t *)realloc(prod1_cache, new_cap * sizeof(seq_t));
    seq_t *new2 = (seq_t *)realloc(prod2_cache, new_cap * sizeof(seq_t));
    if (!new1 || !new2)
        exit(1);

    prod1_cache = new1;
    prod2_cache = new2;
    prod_len    = (idx_t)new_cap;

    /* If we are starting from scratch, initialise the base case */
    if (old_len == 0) {
        prod1_cache[0] = (seq_t)1;
        prod2_cache[0] = (seq_t)1;
        old_len = 1;                    /* we now have index 0 filled */
    }

    /* Compute products for new indices */
    for (idx_t i = old_len; i <= n; ++i) {
        seq_t f1 = (seq_t)(((uint64_t)4 * i - 1) * ((uint64_t)4 * i - 1));
        seq_t f2 = (seq_t)(((uint64_t)4 * i - 3) * ((uint64_t)4 * i - 3));
        prod1_cache[i] = prod1_cache[i - 1] * f1;
        prod2_cache[i] = prod2_cache[i - 1] * f2;
    }
}

/* Ensure a_cache contains terms up to index n (inclusive). */
static void ensure_a(idx_t n)
{
    if (a_len > n)
        return;

    idx_t old_len = a_len;
    size_t new_cap = (size_t)(n + 1);

    seq_t *newa = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!newa)
        exit(1);
    a_cache = newa;
    a_len   = (idx_t)new_cap;

    for (idx_t i = old_len; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;   /* a(0) = 1 */
            continue;
        }

        ensure_prod(i);

        seq_t term1 = prod1_cache[i];
        seq_t sum   = (seq_t)0;

        for (idx_t m = 0; m < i; ++m) {
            seq_t binom_val = binomial((idx_t)(2 * i + 1), (idx_t)(2 * m + 1));
            seq_t prod_term = prod2_cache[i - m];
            seq_t tmp = binom_val * prod_term;
            tmp = tmp * a_cache[m];
            sum += tmp;
        }
        a_cache[i] = term1 - sum;
    }
}

seq_t a(idx_t n)
{
    /* n is unsigned, so n < 0 cannot occur */
    ensure_a(n);
    return a_cache[n];
}
