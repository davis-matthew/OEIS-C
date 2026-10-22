/*
 * A391409  Discriminants of real quadratic fields F such that 16 is an 8th
 *          power in F_2, where F_2 is the completion of F with respect to a
 *          place above 2.
 * https://oeis.org/A391409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th positive fundamental discriminant d > 0 with d
 *            ≡ 8,28,56,60 (mod 64)
 * Range:     u8 n <= 13, u16 n <= 3323, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"

static seq_t *cache = NULL;
static idx_t cache_capacity = 0;
static idx_t terms_found = 0;
static seq_t last_candidate = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n < 1 */

    /* Initial cache allocation */
    if (cache_capacity == 0) {
        cache_capacity = 16;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
    }

    /* Ensure capacity for n terms */
    while (cache_capacity < n) {
        cache_capacity <<= 1; /* double size */
        cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
    }

    /* Generate terms up to n */
    while (terms_found < n) {
        last_candidate += 4; /* check only multiples of 4 */
        seq_t rem = last_candidate % 64;
        if (rem == 8 || rem == 28 || rem == 56 || rem == 60) {
            seq_t m = last_candidate / 4;
            if (is_squarefree(m)) {
                cache[terms_found++] = last_candidate;
            }
        }
    }

    return cache[n - 1];
}
