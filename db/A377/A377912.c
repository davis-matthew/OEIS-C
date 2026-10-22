/*
 * A377912  Numbers such that every even digit except the last is immediately
 *          followed by a strictly larger digit.
 * https://oeis.org/A377912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 172, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A377912: Numbers such that every even digit except the last
   is immediately followed by a strictly larger digit.
   Implementation uses on‑demand generation with memoisation.
*/
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_to_str.h"

/* Helper: check whether a given seq_t number satisfies the digit rule */
static bool valid_num(seq_t x)
{
    /* Handle 0: it trivially satisfies the rule */
    if (x == 0)
        return true;

    char buf[256];
    size_t len;
    seq_to_str(x, buf, &len);

    for (size_t i = 0; i + 1 < len; ++i) {
        int d  = buf[i]   - '0';
        int nd = buf[i+1] - '0';
        if ((d & 1) == 0 && nd <= d)   /* even digit followed by <= digit */
            return false;
    }
    return true;
}

/* Public entry point: sequence value at position n (1‑based) */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static seq_t next_candidate = 0;

    /* Extend cache until we have at least n elements */
    while (cache_len < n) {
        seq_t val = next_candidate;

        if (valid_num(val)) {
            /* Grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 1024;
                if (new_cap < cache_len + 10) new_cap = cache_len + 10;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
