/*
 * A272638  a(1)=1, a(2)=4; thereafter a(n) = squarefree part of
 *          a(n-1)+a(n-2).
 * https://oeis.org/A272638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=4; thereafter a(n)=squarefree part of a(n-1)+a(n-2)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic array of terms (0‑based) */
static idx_t cache_size = 0;         /* number of stored terms */
static size_t cache_cap  = 0;        /* allocated capacity */

/* Ensure the cache can hold at least `need` terms.  Expands as necessary. */
static void ensure_cache(idx_t need)
{
    if (cache_cap < (size_t)need) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap < (size_t)need) new_cap <<= 1;
        /* realloc accepts a void*; cast explicitly for C++. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) exit(1);   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
    /* initialise the first two terms on first use */
    if (cache_size < 2) {
        cache[0] = 1;
        cache[1] = 4;
        cache_size = 2;
    }
}

/* Return the square‑free kernel of v: product of its distinct prime factors. */
static seq_t squarefree_part(seq_t v)
{
    if (v <= 1) return v;
    seq_t result = 1;
    seq_t rem    = v;

    /* test all potential prime divisors p until p*p > rem */
    for (idx_t p = 2; (seq_t)(p * p) <= rem; ++p) {
        if (rem % (seq_t)p == 0) {
            int exp = 0;
            while (rem % (seq_t)p == 0) {
                rem /= (seq_t)p;
                exp++;
            }
            if (exp & 1)            /* odd exponent → include p once */
                result *= (seq_t)p;
        }
    }
    /* if anything remains it is a prime with exponent 1 */
    if (rem > 1) result *= rem;
    return result;
}

/* OEIS required function */
seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 4;

    ensure_cache(n);

    /* compute terms up to n lazily */
    while (cache_size < n) {
        seq_t sum = cache[cache_size - 1] + cache[cache_size - 2];
        cache[cache_size] = squarefree_part(sum);
        cache_size++;
    }
    return cache[n - 1];
}
