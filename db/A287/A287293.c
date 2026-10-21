/*
 * A287293  Golomb's sequence with powers of 2.
 * https://oeis.org/A287293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 204, u32 n <= 1260, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A287293 – Golomb’s sequence over powers of 2
   a(1) = 2.
   Whenever the exponent k (k ≥ 1) is reached, 2^k is repeated
   exactly a(k) times, where a(k) is the term of the sequence at index k.
   The sequence is generated iteratively and cached for arbitrary order
   of queries. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

static seq_t *cache = NULL;   /* cached terms */
static idx_t cached_n = 0;    /* number of terms currently stored (1‑based) */
static idx_t cap = 0;         /* allocated capacity */
static idx_t current_exp = 1; /* exponent of the block currently being output */
static idx_t rem = 0;         /* remaining occurrences of current_exp */

/* Ensure that terms up to index n are computed and cached. */
static void ensure_cache(idx_t n)
{
    if (cached_n >= n)          /* already have the requested terms */
        return;

    /* Make sure there is enough storage. */
    if (n > cap) {
        idx_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* First time initialisation. */
    if (cached_n == 0) {
        cache[0] = pow2(1);           /* a(1) = 2 */
        current_exp = 1;
        rem = (idx_t)cache[0] - 1;    /* one occurrence already used */
        cached_n = 1;
    }

    /* Compute missing terms. */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        if (rem == 0) {              /* start a new exponent block */
            ++current_exp;
            rem = (idx_t)cache[current_exp - 1];
        }
        cache[i - 1] = pow2(current_exp);
        rem--;
    }

    cached_n = n;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n - 1];
}
