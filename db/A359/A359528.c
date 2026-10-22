/*
 * A359528  Nonnegative numbers k such that if 2^i and 2^j appear in the
 *          binary expansion of k, then 2^(i AND j) also appears in the binary
 *          expansion of k (where AND denotes the bitwise AND operator).
 * https://oeis.org/A359528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A359528: numbers k for which the set of binary exponents is
   closed under bitwise AND.  The sequence starts 0,1,2,3,4,5,7,8,... */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Test whether k satisfies the intersection‑closed condition.  */
static bool is_valid(seq_t k)
{
    /* Collect the positions of set bits.  */
    idx_t pos_list[128];
    size_t cnt = 0;
    seq_t t = k;
    idx_t pos = 0;
    while (t != 0) {
        if ((t & 1) != 0) {
            pos_list[cnt++] = pos;
        }
        t >>= 1;
        pos++;
    }

    /* For every pair of set bits, check that their AND position is also set. */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            idx_t ij = pos_list[i] & pos_list[j];
            if (((k >> ij) & 1) == 0) {
                return false;
            }
        }
    }
    return true;
}

/* Return the n‑th term of the sequence (a(1)=0).  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static size_t cache_len = 0;  /* number of terms cached */
    static size_t cache_cap = 0;  /* capacity of the cache */

    if (n == 0) {                 /* undefined, return 0 for safety */
        return 0;
    }

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t cand = (cache_len == 0) ? 0 : cache[cache_len - 1] + 1;

        /* Find next valid term. */
        while (!is_valid(cand)) {
            cand++;
        }

        /* Ensure capacity. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failure – return 0 */
                free(cache);
                cache = NULL;
                cache_cap = 0;
                cache_len = 0;
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = cand;
    }

    return cache[(size_t)n - 1];
}
