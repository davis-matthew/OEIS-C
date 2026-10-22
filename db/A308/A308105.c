/*
 * A308105  Numbers m such that m is greater than the sum of the k-th powers
 *          of its digits, where k is the number of digits of m.
 * https://oeis.org/A308105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 5478, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_pow, is_resistant
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A308105: the n‑th integer m such that
   m > sum of its k-th powers, where k is the number of decimal
   digits of m.  The function uses a simple incremental search
   and memoises the found terms.  It works for built‑in integer
   types and for arbitrary‑precision seq_t in C++17. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_pow.h"
#include "../utils/is_resistant.h"

/* Cache of already computed terms.  The cache is expanded on demand.
   The last number examined is stored in last_checked; searching
   starts from last_checked+1. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated size of cache          */
static idx_t cache_cnt   = 0;   /* number of stored terms           */
static seq_t last_checked = 9;  /* 9 is the largest candidate
                                 * examined before the first call   */

/* Return the n-th term of A308105 (n >= 1).  The result is cached so
   the function works correctly regardless of the order of calls. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1, but 0 is not used */

    if (cache == NULL) {    /* initialise cache */
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_cnt < n) {
        seq_t candidate = last_checked + 1;
        last_checked = candidate;

        if (is_resistant(candidate)) {
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort(); /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = candidate;
        }
    }
    return cache[n - 1];
}
