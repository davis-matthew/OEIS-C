/*
 * A375379  Value of the power tower formed by the numbers obtained by adding
 *          1 to the terms in the n-th composition (in standard order).
 * https://oeis.org/A375379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A065120(n)+1)^a(A053645(n)) for n >= 1, a(0) = 1
 * Range:     u8 n <= 12, u16 n <= 14, u32 n <= 26, u64 n <= 26, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/big_pow.h"

/* Cache for computed values of a(n).  cache[0] is never stored. */
static seq_t *cache    = NULL;
static size_t   cache_cap = 0;

/* Recursive helper that memoises results. */
static seq_t compute_a(idx_t n)
{
    if (n == 0)                      /* a(0) = 1 */
        return (seq_t)1;

    /* Return cached result if available. */
    if (n < cache_cap && cache[n] != (seq_t)0)
        return cache[n];

    /* Find the sum s of the composition (s-th antidiagonal of compositions). */
    idx_t s = 1;
    while (((idx_t)1 << s) - 1 < n)    /* 2^s - 1 >= n */
        ++s;

    /* Number of cut positions within this sum. */
    idx_t len = s - 1;

    /* Offset (0‑based) of the composition inside the block for sum s. */
    idx_t offset = n - ((idx_t)1 << (s - 1));
    idx_t bits   = offset;            /* bits are ordered MSB → LSB */

    idx_t first_part = 0;             /* value of the first part of the composition */
    idx_t suffix_idx = 0;             /* index of the remainder composition */

    if (len == 0) {                   /* sum = 1, composition is (1) */
        first_part = 1;
        suffix_idx = 0;
    } else {
        /* Scan the bits to locate the first cut. */
        int first_cut = -1;
        idx_t part_len = 1;           /* length of the current part */

        for (int i = 0; i < len; ++i) {
            bool bit = ( (bits >> (len - 1 - i)) & 1 );
            if (bit) {
                if (first_cut == -1) {
                    first_cut = i;
                    first_part = part_len;
                }
                part_len = 1;        /* start next part */
            } else {
                ++part_len;           /* extend current part */
            }
        }

        if (first_cut == -1) {        /* no cuts ⇒ single part of size s */
            first_part = len + 1;     /* which equals s */
            suffix_idx = 0;
        } else {
            idx_t r = s - first_part;                   /* remaining sum */
            int suffix_len = (int)(len - first_cut - 1); /* bits after the first cut */
            idx_t suffix_bits = bits & (((idx_t)1 << suffix_len) - 1);

            /* Global index of the suffix composition. */
            suffix_idx = ((idx_t)1 << (r - 1)) + suffix_bits;   /* r >= 1 */
        }
    }

    /* Compute the value using the recursive definition. */
    seq_t base = (seq_t)(first_part + 1);
    seq_t exp  = compute_a(suffix_idx);
    seq_t result = big_pow(base, exp);

    /* Store in cache. */
    if (n >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)calloc(new_cap, sizeof(seq_t));
        if (new_cache == NULL)
            exit(1);                /* allocation failure */
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[n] = result;

    return result;
}

/* External function requested by the exercise. */
seq_t a(idx_t n)
{
    return compute_a(n);
}
