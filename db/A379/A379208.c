/*
 * A379208  Numbers k such that prime(k) and prime(k) + 9 are anagrams.
 * https://oeis.org/A379208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 2279, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts, get_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: k such that prime(k) and prime(k)+9 are anagrams. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/get_prime.h"
#include "../utils/digit_counts.h"

static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static size_t seq_cache_len = 0;
static idx_t last_checked_k = 0u;

/* Extend the cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (seq_cache_len < (size_t)n)
    {
        /* Check the next prime index. */
        last_checked_k++;
        uint32_t p = get_prime(last_checked_k);
        seq_t q = (seq_t)p + (seq_t)9;

        unsigned char cnt_p[10] = {0};
        unsigned char cnt_q[10] = {0};
        digit_counts((seq_t)p, cnt_p);
        digit_counts(q, cnt_q);

        bool same = true;
        for (int d = 0; d < 10; d++)
        {
            if (cnt_p[d] != cnt_q[d])
            {
                same = false;
                break;
            }
        }

        if (same)
        {
            /* Ensure there is room in seq_cache. */
            if (seq_cache_len >= seq_cache_cap)
            {
                size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 8u;
                seq_t *tmp = seq_realloc(seq_cache, seq_cache_cap, new_cap);
                seq_cache = tmp;
                seq_cache_cap = new_cap;
            }
            seq_cache[seq_cache_len++] = (seq_t)last_checked_k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, but harmless */

    ensure_cache(n);
    return seq_cache[n - 1];
}
