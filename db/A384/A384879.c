/*
 * A384879  Numbers whose binary indices have all distinct lengths of maximal
 *          anti-runs (increasing by more than 1).
 * https://oeis.org/A384879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: Numbers whose binary indices have all distinct lengths of maximal anti‑runs. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Check whether a number n satisfies the condition:
   its binary indices (positions of 1 bits, counted from 1 at LSB)
   form maximal anti‑runs, and all run lengths are distinct. */
static inline bool is_good(idx_t n)
{
    seq_t tmp = (seq_t)n;          /* iterate through bits of n                     */
    bool seen[129] = {0};          /* run lengths up to 128 bits                    */
    int run_len = 0;
    bool first_set = false;
    int prev_pos = 0;
    int pos = 1;                   /* 1‑based bit position                         */

    while (tmp) {
        if (tmp & (seq_t)1) {      /* bit is 1                                       */
            if (!first_set) {      /* first set bit found                           */
                first_set = true;
                prev_pos = pos;
                run_len = 1;
            } else {
                if (pos == prev_pos + 1) {     /* consecutive -> end run               */
                    if (seen[run_len]) return false;
                    seen[run_len] = true;
                    run_len = 1;
                } else {                         /* non‑consecutive -> same run         */
                    run_len++;
                }
                prev_pos = pos;
            }
        }
        pos++;
        tmp >>= 1;
    }

    if (!first_set) return true;  /* zero, should not occur for positive integers */

    /* record last run length */
    if (seen[run_len]) return false;
    return true;
}

/* Return the nth term of A384879 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached sequence values                      */
    static size_t cache_cap = 0;    /* allocated capacity                          */
    static size_t cache_len = 0;    /* number of terms stored                      */
    static idx_t last_checked = 0;  /* last integer examined                       */

    if (n <= cache_len) return cache[n - 1];

    for (idx_t x = last_checked + 1; cache_len < n; ++x) {
        if (is_good(x)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failure (unlikely)          */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)x;
        }
        last_checked = x;
    }
    return cache[n - 1];
}
