/*
 * A383669  Numbers whose binary representation has a positive number of 0's,
 *          all with odd runlength.
 * https://oeis.org/A383669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383669:
   numbers whose binary representation has a positive number of 0's,
   all with odd runlength.  Computed by brute‑force search with memoization. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"   /* provides unsigned bitlen_seq_t(seq_t) */

static bool has_odd_zero_runs(seq_t x)
{
    /* Must contain at least one zero. */
    if (x <= 1)
        return false;

    unsigned len = bitlen_seq_t(x);          /* number of bits in x */
    bool in_zero_run = false;
    unsigned zero_len = 0;
    bool found_zero = false;

    /* Scan bits from most‑significant to least‑significant. */
    for (unsigned i = len; i-- > 0; ) {
        bool bit = ((x >> i) & 1u) != 0;
        if (!bit) {
            in_zero_run = true;
            zero_len++;
        } else {
            if (in_zero_run) {
                if ((zero_len & 1u) == 0u)   /* even zero run */
                    return false;
                found_zero = true;
                in_zero_run = false;
                zero_len   = 0u;
            }
        }
    }
    if (in_zero_run) {                       /* trailing zero run */
        if ((zero_len & 1u) == 0u)
            return false;
        found_zero = true;
    }
    return found_zero;
}

seq_t a(idx_t n)
{
    /* Lazy memoization – works for arbitrary call order */
    static seq_t *cache     = NULL;
    static size_t cache_sz  = 0;
    static size_t cache_cap = 0;
    static seq_t next_value  = 2;            /* next integer to test */

    while (cache_sz < (size_t)n) {
        if (has_odd_zero_runs(next_value)) {
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = next_value;
        }
        next_value++;                         /* move to the next integer */
    }

    return cache[(size_t)(n - 1)];
}
