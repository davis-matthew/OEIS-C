/*
 * A317291  Numbers k not in A239019 such that A177894(k) = 0.
 * https://oeis.org/A317291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of A317291.
   a(1) = 0. For n ≥ 2, a(n) is the (n‑2)-th integer k ≥ 1000
   whose decimal representation (last 4 digits) a,b,c,d satisfies
        a + c = b + d
   and is not of the form a,b,a,b (i.e., not a==c && b==d).
   The terms are generated in ascending numerical order by simple enumeration. */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdbool.h>  /* bool, true, false */

static bool is_valid(idx_t k)
{
    if (k < 1000) return false;
    idx_t a = (k / 1000) % 10;
    idx_t b = (k / 100) % 10;
    idx_t c = (k / 10) % 10;
    idx_t d = k % 10;
    if (a + c != b + d) return false;
    if (a == c && b == d) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Handle the special first term. */
    if (n == 1) return 0;

    /* Index of the desired term in the 4‑digit sequence (starting from 0). */
    size_t idx = n - 2;

    /* Static cache of already discovered terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1000; /* Next integer to test for validity. */

    /* If the required term is already cached, return it. */
    if (idx < cache_len) return cache[idx];

    /* Grow the cache until it contains the requested term. */
    while (cache_len <= idx) {
        /* Find the next valid integer. */
        while (!is_valid(next_k)) ++next_k;

        /* Ensure enough capacity. */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* Allocation failed – fallback to 0. */
                free(cache);
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the valid integer and move to the next candidate. */
        cache[cache_len] = (seq_t)next_k;
        ++cache_len;
        ++next_k;
    }

    return cache[idx];
}
