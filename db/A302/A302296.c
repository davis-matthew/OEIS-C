/*
 * A302296  Positive numbers that can be written in exactly one way as i*j*k
 *          with i < j < 2*i.
 * https://oeis.org/A302296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers that can be written in exactly one way as i*j*k with i <
 *            j < 2*i (OEIS A302296)
 * Range:     u8 n <= 38, u16 n <= 9529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   one_factorisation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/one_factorisation.h"

/* cached sequence storage */
static seq_t   *cache = NULL;   /* array of terms */
static size_t   cached = 0;     /* number of cached terms */
static size_t   cap    = 0;     /* current capacity of cache */
static uint64_t last_val = 5;   /* last value already examined */

/* resize cache to at least new_cap */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cap) return;
    size_t new_cap_val = new_cap;
    if (new_cap_val == 0) new_cap_val = 16;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap_val * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;
    cap   = new_cap_val;
}

/* generate terms until we have at least n terms cached */
static void generate_up_to(idx_t n)
{
    while (cached < (size_t)n) {
        ++last_val;
        if (one_factorisation(last_val))
        {
            ensure_capacity(cached + 1);
            cache[cached++] = (seq_t)last_val;
        }
    }
}

seq_t a(idx_t n)
{
    /* n is 1-based; ignore 0 and negative indices            */
    if (n == 0) return 0;   /* out of range, but harmless */
    generate_up_to(n);
    return cache[(size_t)n - 1];
}
