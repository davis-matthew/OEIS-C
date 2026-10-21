/*
 * A247100  The number of ways to write an n-bit binary string and then define
 *          each run of ones as an element in an equivalence relation.
 * https://oeis.org/A247100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..ceil(n/2)} binomial(n+1, 2k)*Bell(k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 40, u128 n <= 71,
 *            big any n
 * Helpers:   binomial, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 is the first term. */
#include <stdlib.h>
#include "../utils/binomial.h"    /* binomial(idx_t a, idx_t b) returns seq_t */
#include "../utils/seq_realloc.h" /* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) returns seq_t* */

static seq_t *Bell = NULL;      /* Bell[0..Bell_cap-1] */
static idx_t   Bell_cap = 0;

/* Ensure Bell numbers are available up to index k. */
static void ensure_bell(idx_t k)
{
    if (k < Bell_cap)
        return;
    /* Reallocate with new capacity */
    idx_t new_cap = k + 1;
    Bell = seq_realloc(Bell, Bell_cap, new_cap);
    if (!Bell)
        abort();  /* Allocation failure – terminate. */
    for (idx_t i = Bell_cap; i < new_cap; ++i) {
        if (i == 0) {
            Bell[0] = (seq_t)1;
        } else {
            seq_t sum = 0;
            for (idx_t j = 0; j < i; ++j) {
                seq_t b = binomial(i - 1, j);
                sum += Bell[j] * b;
            }
            Bell[i] = sum;
        }
    }
    Bell_cap = new_cap;
}

/* Compute a(n) directly from the defining sum. */
static seq_t compute_a_raw(idx_t n)
{
    idx_t maxK = (n + 1) / 2;          /* ceil(n/2) */
    ensure_bell(maxK);                /* need Bell[0..maxK] */

    seq_t sum = (seq_t)1;              /* start with the outer +1 */

    seq_t c = (seq_t)1;                /* binomial(n+1,0) */
    for (idx_t j = 1; j <= n + 1; ++j) {
        /* Update binomial(n+1, j): c = c * (n+2-j) / j; */
        c = c * (seq_t)(n + 2 - j);
        c /= (seq_t)j;                 /* exact division */
        if ((j & 1) == 0 && j >= 2) {  /* even j, k=j/2 >=1 */
            idx_t k = j >> 1;
            sum += c * Bell[k];
        }
    }
    return sum;
}

/* Cache for a(n) values. */
static seq_t *a_cache = NULL;      /* a_cache[0..a_cache_cap-1] */
static idx_t   a_cache_cap = 0;

/* Precompute a(x) for all x <= n and store them. */
static void ensure_a_cache(idx_t n)
{
    if (n < a_cache_cap)
        return;
    /* Reallocate with new capacity */
    idx_t new_cap = n + 1;
    a_cache = seq_realloc(a_cache, a_cache_cap, new_cap);
    if (!a_cache)
        abort();
    for (idx_t i = a_cache_cap; i < new_cap; ++i) {
        a_cache[i] = compute_a_raw(i);
    }
    a_cache_cap = new_cap;
}

/* Public interface. */
seq_t a(idx_t n)
{
    ensure_a_cache(n);
    return a_cache[n];
}
