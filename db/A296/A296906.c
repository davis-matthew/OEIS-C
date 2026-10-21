/*
 * A296906  Numbers whose base-60 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          = #(peaks); see Comments.
 * https://oeis.org/A296906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: A296906 – numbers whose base‑60 digits have
   the same number of pits and peaks.  The function returns
   the nth term (1‑indexed) by generating the sequence on
   demand. */

#include <stdbool.h>
#include <stdlib.h>

/* static cache of computed sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;       /* number of stored elements */
static size_t cache_cap = 0;       /* current capacity of cache */
static seq_t next_check = (seq_t)1; /* next integer to test */

/* check whether a single number satisfies the pit‑equals‑peak rule */
static bool is_valid(seq_t n)
{
    seq_t temp = n;
    unsigned digits[32];   /* enough for all values we will encounter */
    size_t len = 0;

    if (temp == 0) {
        digits[len++] = 0;
    } else {
        while (temp > 0) {
            seq_t rem = temp % 60u;
            unsigned r = (unsigned)(long long)rem; /* remainder < 60 */
            digits[len++] = r;
            temp = temp / 60u;
        }
    }

    if (len <= 2) return true;  /* no interior indices */

    size_t pits = 0, peaks = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        if (digits[i-1] > digits[i] && digits[i] < digits[i+1]) ++pits;
        else if (digits[i-1] < digits[i] && digits[i] > digits[i+1]) ++peaks;
    }
    return pits == peaks;
}

/* nth term of sequence A296906 (a(1) = 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; unused otherwise */

    /* extend cache until we have at least n elements */
    while (cache_len < n) {
        seq_t val = next_check;
        if (is_valid(val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;   /* allocation failure – should not happen */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        next_check = next_check + 1;
    }
    return cache[n-1];
}
