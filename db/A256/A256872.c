/*
 * A256872  Numbers whose binary expansion is the concatenation of the binary
 *          expansion of two prime numbers in at least two ways.
 * https://oeis.org/A256872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   bitlen_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th odd integer >2 whose binary representation can
   be split into two prime binary parts in at least two distinct ways. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_prime_big.h"

/* a(idx_t n) – returns the n‑th term of the sequence (1‑based index) */
seq_t a(idx_t n)
{
    /* Lazy static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t  last_scanned = 0;   /* last integer examined */

    if (n == 0) return 0;            /* undefined for n < 1 */

    /* Grow cache capacity to accommodate at least 'n' elements */
    if (cache_len < n) {
        size_t need = n;
        if (cache_cap < need) {
            size_t newcap = cache_cap ? cache_cap * 2 : 16;
            while (newcap < need) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();    /* allocation failed */
            cache       = tmp;
            cache_cap   = newcap;
        }

        /* Scan successive odd integers until we have at least n terms */
        while (cache_len < n) {
            last_scanned++;

            /* Skip 0,1,2 and even numbers – they cannot be a concatenation */
            if (last_scanned <= 2) continue;
            if ((last_scanned & 1) == 0) continue;

            seq_t cand = (seq_t)last_scanned;
            int ways = 0;
            idx_t len = bitlen_seq_t(cand);

            for (idx_t k = 1; k < len; ++k) {
                /* Split after k most significant bits:
                   left  = first k bits
                   right = remaining len-k bits */
                seq_t left  = cand >> (len - k);
                seq_t mask  = ((seq_t)1 << (len - k)) - 1;
                seq_t right = cand & mask;

                /* Both parts must be primes ≥ 2 and no leading zeros */
                if (left <= 1 || right <= 1) continue;
                /* right must start with a 1-bit */
                if (right < ((seq_t)1 << (len - k - 1))) continue;

                if (is_prime_big(left) && is_prime_big(right)) {
                    ++ways;
                    if (ways >= 2) break;   /* at least two distinct decompositions */
                }
            }

            if (ways >= 2) {
                cache[cache_len++] = cand;
            }
        }
    }

    return cache[n - 1];   /* 1‑based index */
}
