/*
 * A279430  Numbers k such that k^2 has an odd number of digits in base 2 and
 *          the middle digit is 0.
 * https://oeis.org/A279430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the list of integers k such that the binary
 * representation of k^2 has an odd number of bits and the middle
 * bit is 0.  The function returns the n‑th such integer, with
 * a(1) = 0.  The implementation keeps a static cache of all terms
 * that have been generated so far, so that calls with non‑monotonic
 * n are handled efficiently. */

#ifdef __cplusplus
#include <vector>
#include <cstddef>
#else
#include <stdlib.h>
#include <stddef.h>
#endif

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h" /* bitlen_seq_t(seq_t) */

seq_t a(idx_t n)
{
    /* Perform the search from k = 0 upwards, caching the terms we find. */
    static idx_t next_k = 0;   /* next k to test */

#ifdef __cplusplus
    /* Use a C++ vector when seq_t is a proper class. */
    static std::vector<seq_t> cache;
    while ((idx_t)cache.size() < n)
    {
        bool accept = false;
        seq_t candidate;

        if (next_k == 0)          /* k = 0 always qualifies */
        {
            accept = true;
            candidate = 0;
        }
        else
        {
            seq_t k = (seq_t)next_k;
            seq_t sq = k * k;    /* compute k^2 */

            unsigned int len = bitlen_seq_t(sq);
            if (len > 0)          /* len is number of bits in sq */
            {
                unsigned int bitlen_minus_one = len - 1; /* floor(log₂(sq)) */
                if ((bitlen_minus_one & 1) == 0)        /* odd length */
                {
                    unsigned int m = bitlen_minus_one >> 1; /* middle bit pos */
                    if ((((sq >> m) & 1) == 0))            /* middle bit 0 */
                    {
                        accept = true;
                        candidate = k;
                    }
                }
            }
        }

        if (accept)
            cache.push_back(candidate);

        ++next_k;
    }
    return cache[n - 1];
#else
    /* Plain C implementation using realloc. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    while (cache_len < n)
    {
        bool accept = false;
        seq_t candidate;

        if (next_k == 0)
        {
            accept = true;
            candidate = 0;
        }
        else
        {
            seq_t k = (seq_t)next_k;
            seq_t sq = k * k;

            unsigned int len = bitlen_seq_t(sq);
            if (len > 0)
            {
                unsigned int bitlen_minus_one = len - 1;
                if ((bitlen_minus_one & 1) == 0)
                {
                    unsigned int m = bitlen_minus_one >> 1;
                    if ((((sq >> m) & 1) == 0))
                    {
                        accept = true;
                        candidate = k;
                    }
                }
            }
        }

        if (accept)
        {
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        ++next_k;
    }
    return cache[n - 1];
#endif
}
