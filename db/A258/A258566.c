/*
 * A258566  Triangle in which n-th row contains all possible products of n-1
 *          of the first n primes in descending order.
 * https://oeis.org/A258566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (Π_{i=1}^{r} p_i) / p_k * where r is the row such that
 *            r(r‑1)/2 < n ≤ r(r+1)/2 * and k = n – r(r‑1)/2. p_i denotes the
 *            i‑th prime
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"

/* Cached cumulative prime products, 1‑based. */
static seq_t *prod = NULL;
static idx_t prod_cap = 0;

/* Ensure prod[1..r] are initialized. */
static void ensure_prod(idx_t r)
{
    if (prod_cap >= r)
        return;
    size_t new_cap = (size_t)(r + 1);             /* index 0 unused */
    seq_t *tmp = (seq_t *)realloc(prod, new_cap * sizeof(seq_t));
    if (!tmp)                                   /* fatal if memory exhausted */
        exit(1);
    prod = tmp;
    for (idx_t i = prod_cap + 1; i <= r; ++i) {
        seq_t p = (seq_t)get_prime(i);
        if (i == 1)
            prod[1] = p;
        else
            prod[i] = prod[i - 1] * p;
    }
    prod_cap = r;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;    /* sequence offset 1 */

    /* Find the row r: r(r+1)/2 ≥ n. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Position k within the row (1‑based). */
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t k = n - prev_sum;

    ensure_prod(r);

    return prod[r] / (seq_t)get_prime(k);
}
