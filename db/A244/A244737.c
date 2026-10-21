/*
 * A244737  Numbers k such that (prime(k) mod 5) is odd.
 * https://oeis.org/A244737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244737: indices k such that prime(k) mod 5 is odd */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

/* Static cache of computed terms */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;
static uint64_t next_k = 1;   /* next prime index to test */

/* Return the n‑th element of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (!cache_cap) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        uint32_t p  = get_prime(next_k);         /* prime(next_k) */
        uint32_t r  = p % 5;                     /* residue mod 5 */
        if (r & 1) {                             /* odd residue (1 or 3) */
            if (cache_size == cache_cap) {       /* grow cache if full */
                size_t new_cap = cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;       /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)next_k;  /* store the index k */
        }
        ++next_k;
    }

    return cache[n - 1];
}
