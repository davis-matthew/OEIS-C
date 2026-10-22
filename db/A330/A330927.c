/*
 * A330927  Numbers k such that both k and k + 1 are Niven numbers.
 * https://oeis.org/A330927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer k such that both k and k+1 are
 *            Niven numbers. Computed by linear scan with memoisation
 * Range:     u8 n <= 18, u16 n <= 686, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Check whether x is a Niven number */
static inline bool is_niven(seq_t x)
{
    if (x == 0) return false;
    seq_t tmp = x;
    seq_t sum = 0;
    while (tmp > 0) {
        sum += tmp % 10;
        tmp /= 10;
    }
    return x % sum == 0;
}

/* Return the n-th term of sequence A330927 (1‑based index) */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t next = 1;   /* next integer to test as k */

    if (n == 0) return 0;    /* undefined for n < 1, but guard against call */

    while (cache_size < n) {
        seq_t cur = (seq_t)next;
        seq_t cur_next = cur + 1;
        if (is_niven(cur) && is_niven(cur_next)) {
            /* grow cache if needed */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                /* In case of allocation failure, abort */
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cur;
        }
        ++next;
    }

    return cache[n - 1];
}
