/*
 * A248528  Numbers k such that the smallest prime divisor of k^2+1 is 17.
 * https://oeis.org/A248528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1958, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A248528: numbers k such that the smallest prime divisor of k^2+1 is 17.
   The code searches for admissible k, caching the results so that calls can be 
   made in any order.  It tests the congruence conditions
   k ≡ 4 or 13 (mod 17) and excludes k ≡ 2,3 (mod 5) and k ≡ 5,8 (mod 13).  */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for generated terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* next candidate to test (will be increased after each trial) */
    static seq_t next_candidate = 4;   /* first possible term is 4 */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* find the next valid k */
        seq_t cand = next_candidate;
        for (;;) {
            /* candidate must be even */
            if ((cand % 2) != 0) { ++cand; continue; }

            /* exclude k ≡ 2 or 3 (mod 5)  (k^2+1 is divisible by 5) */
            seq_t r5 = cand % 5;
            if (r5 == 2 || r5 == 3) { ++cand; continue; }

            /* exclude k ≡ 5 or 8 (mod 13)  (k^2+1 is divisible by 13) */
            seq_t r13 = cand % 13;
            if (r13 == 5 || r13 == 8) { ++cand; continue; }

            /* accept only if k ≡ 4 or 13 (mod 17)  (k^2+1 divisible by 17) */
            seq_t r17 = cand % 17;
            if (r17 != 4 && r17 != 13) { ++cand; continue; }

            /* accepted term */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();              /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
            next_candidate = cand + 1;
            break; /* stop searching for this term */
        }
    }
    return cache[n - 1];
}
