/*
 * A349094  a(n) = 2^(n-1) - tau(n) where tau(n) is the number of divisors of
 *          n.
 * https://oeis.org/A349094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) - tau(n) where tau(n) = number of divisors of n
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

/* Static cache for divisor counts (1‑based).  The pointer is kept as a void*
   so that the same code compiles for both C and C++ (in C++ we use delete[]). */
static void *tau_cache = NULL;   /* will point to seq_t array */
static idx_t tau_limit = 0;      /* highest n for which tau_cache is complete */

/* Ensure tau_cache[1..n] is fully populated */
static void ensure_tau_cache(idx_t n)
{
    if (n <= tau_limit) return;          /* already computed */

    size_t cap = (size_t)n + 1;          /* +1 to allow indexing by 1..n */

#ifdef __cplusplus
    seq_t *new_cache = new seq_t[cap];
#else
    void *new_cache = malloc(cap * sizeof(seq_t));
#endif
    if (!new_cache) return;              /* allocation failed – trust caller */

    /* Initialize to zero */
    memset(new_cache, 0, cap * sizeof(seq_t));

    seq_t *cache = (seq_t *)new_cache;

    /* Sieve to count divisors */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            cache[j] += (seq_t)1;
        }
    }

    /* Replace old cache */
#ifdef __cplusplus
    delete[] (seq_t *)tau_cache;
#else
    free(tau_cache);
#endif
    tau_cache = new_cache;
    tau_limit = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for out‑of‑range calls */

    ensure_tau_cache(n);
    seq_t *cache = (seq_t *)tau_cache;
    seq_t tau   = cache[n];
    seq_t pow2n = pow2(n - 1);            /* 2^(n-1) in arbitrary precision */
    return pow2n - tau;
}
