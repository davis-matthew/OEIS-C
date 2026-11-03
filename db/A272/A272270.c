/*
 * A272270  Positive integers n where the number of parts function on the set
 *          of 4-ary partitions of n is equidistributed mod 4.
 * https://oeis.org/A272270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of positive integers n for which the base‑4
   representation contains a digit 3 at some position other than
   the least significant digit.  The function computes the nth such
   integer (a(1)=12, a(2)=13, ...).  The result is memoised so that
   each integer is examined only once.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* returns true iff m has a digit 3 at some position i>0 in base‑4 */
static inline bool has_digit3_elsewhere(idx_t m)
{
    if (m < 4) return false;      /* only one digit */
    idx_t pos = 0;
    while (m > 0) {
        if (pos > 0 && (m & 3u) == 3u) return true;
        m >>= 2;                  /* divide by 4 */
        pos++;
    }
    return false;
}

static seq_t *cache = NULL;        /* memoisation array */
static size_t cache_cap = 0;       /* allocated capacity */
static size_t cache_cnt = 0;       /* number of terms stored */
static idx_t  last_checked = 0;    /* last integer examined */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out‑of‑range */
    while (cache_cnt < n) {
        last_checked++;            /* test next integer */
        if (has_digit3_elsewhere(last_checked)) {
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
