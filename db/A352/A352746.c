/*
 * A352746  a(n) is the number of Lyndon factors of the Thue-Morse string of
 *          length n.
 * https://oeis.org/A352746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of Lyndon factors of the Thue‑Morse string of
 *            length n. The algorithm computes the Thue‑Morse prefix using the
 *            parity of the binary representation of each index, then applies
 *            Duval’s algorithm to obtain the factorization and counts the
 *            factors
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static inline uint8_t bit_parity(uint64_t x)
{
    /* Parity of the number of 1‑bits in x. 0 for even, 1 for odd. */
    return (uint8_t)__builtin_parityll(x);
}

/* Return a(n).  The function may be called in any order and
   caches the results for efficiency. */
seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;   /* number of entries stored */

    /* static storage for the Thue–Morse bits generated up to the current
       maximum n; bits[i] is the i‑th bit (0 or 1). */
    static uint8_t *bits     = NULL;
    static size_t bits_cap   = 0;
    static size_t bits_len   = 0;   /* number of bits currently stored */

    /* Ensure we have cached values up to index n. */
    if ((idx_t)cache_len <= n) {
        /* Grow cache array if necessary. */
        size_t req_cap = (size_t)n + 1;
        if (req_cap > cache_cap) {
            cache = (seq_t *)realloc((void *)cache, req_cap * sizeof(seq_t));
            if (!cache) return (seq_t)0;           /* allocation failed */
            cache_cap = req_cap;
        }

        /* Grow bits array if necessary. */
        size_t req_bits_cap = (size_t)n;
        if (req_bits_cap > bits_cap) {
            bits = (uint8_t *)realloc((void *)bits, req_bits_cap * sizeof(uint8_t));
            if (!bits) return (seq_t)0;           /* allocation failed */
            bits_cap = req_bits_cap;
        }

        /* Generate new Thue–Morse bits for indices bits_len .. n-1. */
        for (size_t i = bits_len; i < (size_t)n; ++i)
            bits[i] = bit_parity((uint64_t)i);
        bits_len = (size_t)n;

        /* Compute Lyndon factorization counts for all new prefixes. */
        for (size_t m = cache_len; m <= (size_t)n; ++m) {
            seq_t count = 0;
            if (m == 0) {                /* empty string has 0 factors */
                cache[m] = 0;
                continue;
            }

            size_t i = 0;
            while (i < m) {
                size_t j = i + 1;
                size_t k = i;

                /* Duval's inner loop */
                while (j < m && bits[k] <= bits[j]) {
                    if (bits[k] < bits[j])
                        k = i;
                    else
                        ++k;
                    ++j;
                }

                size_t l = j - k;          /* length of the Lyndon word */

                /* Count every occurrence of the word in the block. */
                while (i <= k) {
                    ++count;
                    i += l;
                }
            }
            cache[m] = count;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
