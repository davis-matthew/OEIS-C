/*
 * A326194  Number of iterations of x -> A009194(x) needed to reach a fixed
 *          point when starting from x = n, where A009194(x) = gcd(x,
 *          sigma(x)).
 * https://oeis.org/A326194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if gcd(n,σ(n)) = n * a(n) = 1 + a(gcd(n,σ(n)))
 *            otherwise * where σ(n) is the sum of divisors of n
 * Range:     u8 n <= 679, u16 n <= 9627, u32 n <= 9627, u64 n <= 9627,
 *            u128 n <= 9627, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *sigma   = NULL;   /* σ values for 0 … cap-1      */
static seq_t *cache   = NULL;   /* a values for 0 … cap-1      */
static bool *computed = NULL;   /* true if cache[n] is valid   */
static size_t cap = 0;          /* current allocated capacity  */

/* Resize the static arrays to hold values up to index n (inclusive).  */
static void _ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cap) return;

    /* Allocate new arrays of the larger size.  */
#ifdef __cplusplus
    seq_t  *new_sigma   = new seq_t[needed];
    seq_t  *new_cache   = new seq_t[needed];
    bool   *new_comp    = new bool[needed];
#else
    seq_t  *new_sigma   = (seq_t*)malloc(needed * sizeof(seq_t));
    seq_t  *new_cache   = (seq_t*)malloc(needed * sizeof(seq_t));
    bool   *new_comp    = (bool*)malloc(needed * sizeof(bool));
#endif

    /* Copy old data into the new arrays.  */
    for (size_t i = 0; i < cap; ++i) {
        new_sigma[i] = sigma[i];
        new_cache[i] = cache[i];
        new_comp[i]  = computed[i];
    }
    /* Initialize the newly allocated slots.  */
    for (size_t i = cap; i < needed; ++i) {
        new_sigma[i] = (seq_t)0;
        new_cache[i] = (seq_t)0;
        new_comp[i]  = false;
    }

    /* Free the old arrays.  */
#ifdef __cplusplus
    delete[] sigma;
    delete[] cache;
    delete[] computed;
#else
    free(sigma);
    free(cache);
    free(computed);
#endif

    sigma   = new_sigma;
    cache   = new_cache;
    computed = new_comp;
    cap = needed;

    /* Recompute σ(j) for all 1 ≤ j < cap using a simple sieve.  */
    for (idx_t i = 1; i < (idx_t)cap; ++i) {
        seq_t add = (seq_t)i;
        for (idx_t j = i; j < (idx_t)cap; j += i) {
            sigma[j] += add;
        }
    }
    /* σ(0) stays 0 by initialization above.  */
}

/* Recursive evaluation of a(n) with memoisation.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for 0; safe fallback */

    _ensure_capacity(n);

    if (computed[n]) return cache[n];

    seq_t s = sigma[n];
    seq_t g = gcd((seq_t)n, s);

    seq_t result;
    if (g == (seq_t)n) {
        result = (seq_t)0;
    } else {
        /* g ≤ n, so this cast is safe for idx_t.  */
        result = (seq_t)1 + a((idx_t)g);
    }

    cache[n] = result;
    computed[n] = true;
    return result;
}
