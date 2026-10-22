/*
 * A316842  Three-column table read by rows giving primitive integer sides of
 *          proper triangles (i,j,k) with i >= j >= k >= 1, j+k > i,
 *          gcd(i,j,k) = 1.
 * https://oeis.org/A316842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   This function returns the n‑th term of the flattened
 *            three‑column table of primitive integer sides of proper
 *            triangles (i,j,k) with i ≥ j ≥ k ≥ 1, j + k > i, and gcd(i,j,k)
 *            = 1
 * Range:     u8 n <= 9000, u16 n <= 9000, u32 n <= 9000, u64 n <= 9000,
 *            u128 n <= 9000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is stored row‑wise: [i,j,k],[i',j',k'],… */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;   /* Cached sequence values            */
static size_t cache_cap = 0;   /* Allocated capacity (in seq_t)     */
static size_t cache_len = 0;   /* Number of stored seq_t elements   */

/* Current generators for the next candidate triple. */
static idx_t gen_i = 1;          /* current i while generating       */
static idx_t gen_j = 1;          /* current j while generating       */
static idx_t gen_k = 1;          /* current k while generating       */

/* Ensure the cache has room for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();                     /* unrecoverable error          */
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate terms until the cache has at least 'target' entries. */
static void generate_until(size_t target)
{
    while (cache_len < target) {
        /* Find the next primitive triple. */
        while (1) {
            if (gen_j > gen_i) {            /* move to next i */
                gen_i++;
                gen_j = 1;
                gen_k = 1;
                continue;
            }
            if (gen_k > gen_j) {            /* move to next j */
                gen_j++;
                gen_k = 1;
                continue;
            }

            seq_t a = (seq_t)gen_i;
            seq_t b = (seq_t)gen_j;
            seq_t c = (seq_t)gen_k;

            /* Check triangle inequality and primitivity. */
            if ((gen_j + gen_k > gen_i) &&
                (gcd(gcd(a, b), c) == (seq_t)1))
                break;                     /* valid triple found */

            gen_k++;                       /* try next k */
        }

        /* Store the triple in cache. */
        ensure_capacity(cache_len + 3);
        cache[cache_len++] = (seq_t)gen_i;
        cache[cache_len++] = (seq_t)gen_j;
        cache[cache_len++] = (seq_t)gen_k;

        /* Prepare for the next search iteration. */
        gen_k++;                           /* start from next k */
    }
}

/* Return the n‑th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                      /* safety for n<1 */

    /* All required terms must be generated. */
    generate_until((size_t)n);

    /* cache[0] holds a(1), so index is n-1. */
    return cache[(size_t)(n - 1)];
}
