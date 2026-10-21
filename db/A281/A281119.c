/*
 * A281119  Number of complete tree-factorizations of n >= 2.
 * https://oeis.org/A281119
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 4607, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(n) = 1                         if n is prime
   a(n) = Σ ∏ a(fi)                 otherwise, where the sum runs over all weakly
   where n = f1⋯fk (k≥2), f1≤…≤fk and each fi>1.
   (These partitions are the weakly increasing factorizations of n.)
   The product is taken over the chosen factors.
   This counts all complete tree‑factorizations whose leaves are prime. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *A_cache = NULL;   /* A_cache[i] = a(i) once computed           */
static size_t A_cap   = 0;      /* current allocated capacity (number of slots) */
static idx_t  computed_up_to = 1;/* largest n computed so far (>=1) */

/* Ensure that the cache can hold values up to at least 'n'.
   The cast is necessary because realloc returns a void*. */
static void ensure_cache(idx_t n)
{
    if (A_cap <= n) {
        seq_t *tmp = (seq_t *)realloc(A_cache, (n + 1) * sizeof(seq_t));
        if (!tmp) exit(1);                /* memory exhausted */
        A_cache = tmp;
        A_cap   = n + 1;
    }
}

/* Recursively add the products of a(f1)…a(fk) for all weakly increasing
   factorizations of 'n' whose first factor is ≥ min_factor.
   'prod' holds the product of a(fi) for factors chosen before the recursion.
   'sum' points to the running total. */
static void dfs(idx_t n, idx_t min_factor, seq_t prod, seq_t *sum)
{
    /* The last factor is 'n'.  Include its contribution. */
    *sum += prod * A_cache[n];

    /* Try all possible next factors d (>= min_factor) that divide n
       and keep the order nondecreasing. */
    for (idx_t d = min_factor; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t rest = n / d;
            if (rest < d) continue;         /* maintain nondecreasing order */
            dfs(rest, d, prod * A_cache[d], sum);
        }
    }
}

/* Compute a(i) for all i up to 'n', storing results in A_cache. */
static void computeUpTo(idx_t n)
{
    if (n <= computed_up_to) return;
    ensure_cache(n);
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if (is_prime(i)) {
            A_cache[i] = 1;
        } else {
            seq_t total = 0;
            /* Start the recursion with the first factor d. */
            for (idx_t d = 2; d * d <= i; ++d) {
                if (i % d == 0) {
                    idx_t rest = i / d;
                    if (rest < d) continue;
                    dfs(rest, d, A_cache[d], &total);
                }
            }
            A_cache[i] = total;
        }
    }
    computed_up_to = n;
}

/* Public sequence function requested by the build system. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* undefined for n < 2 */
    computeUpTo(n);
    return A_cache[n];
}
