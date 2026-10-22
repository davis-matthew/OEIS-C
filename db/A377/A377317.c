/*
 * A377317  Numbers k such that prime(k), prime(k)+6, and prime(k)+12 are
 *          primes.
 * https://oeis.org/A377317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number k such that prime(k), prime(k)+6, and prime(k)+12
 *            are primes (computed by searching through the primes and
 *            recording suitable indices)
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;           /* undefined for offset 1, but harmless */
    }

    /* Cached sequence values (k's) */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* Next prime index to test */
    static idx_t next_k = 1;   /* 1‑based index, starts with prime(1)=2 */

    /* Ensure we have enough space for the cache */
    if (cache_len < (size_t)n) {
        size_t need = (size_t)n;
        grow_seq(&cache, &cache_cap, need);
    }

    /* Generate new terms until we have at least n values */
    while (cache_len < (size_t)n) {
        idx_t  k = next_k++;                     /* current prime index */
        uint32_t p = get_prime(k);               /* prime(k) */

        /* Check the 6‑ and 12‑gap condition */
        if (is_prime((idx_t)(p + 6)) && is_prime((idx_t)(p + 12))) {
            cache[cache_len++] = (seq_t)k;       /* store k */
        }
    }

    return cache[n - 1];   /* 1‑based indexing */
}
