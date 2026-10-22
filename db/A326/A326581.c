/*
 * A326581  Odd integers which are prime or square.
 * https://oeis.org/A326581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth odd integer that is prime or a perfect square
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    /* Static cache of computed sequence values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values stored */
    static size_t cache_cap = 0;   /* allocated capacity of cache    */

    /* State for continued generation */
    static seq_t last_candidate = 0;            /* last odd number processed  */
    static idx_t next_square_k = 1;             /* k such that next odd square is (2*k-1)^2 */
    static seq_t next_square_val = 1;           /* value of next odd square   */

    /* Return immediately if n is zero (undefined) */
    if (n == 0) return 0;

    /* Extend cache until it contains at least n elements */
    while (cache_len < n) {
        /* Ensure enough capacity */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure, return 0 */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Find the next odd integer that is either prime or a perfect square */
        seq_t cand;
        if (last_candidate == 0) cand = 1;           /* start with 1 */
        else cand = last_candidate + 2;              /* next odd after previous */

        while (true) {
            bool is_square = (cand == next_square_val);
            bool is_prime = false;
            if (cand > 2) is_prime = is_prime_small(cand);
            if (is_square || is_prime) break;        /* found next term */

            cand += 2;                               /* skip to next odd */
        }

        /* Store the found term */
        cache[cache_len++] = cand;
        last_candidate = cand;

        /* Advance next odd square if we just used it */
        if (cand == next_square_val) {
            next_square_k++;
            seq_t odd = 2 * next_square_k - 1;
            next_square_val = odd * odd;
        }
    }

    return cache[n - 1];
}
