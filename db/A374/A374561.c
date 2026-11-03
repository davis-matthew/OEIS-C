/*
 * A374561  Integers which are palindromes when expressed in more than one
 *          base 2 to 10.
 * https://oeis.org/A374561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   is_palindrome_base, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A374561.
   Formed by scanning integers and selecting those that are palindromes
   in at least two bases among 2..10.  The values are stored in a
   static array so that repeated calls of a(idx_t n) are fast.   */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;          /* buffer of discovered terms       */
static size_t cached_len = 0;        /* number of terms stored           */
static size_t cache_cap = 0;         /* capacity of buffer                */
static seq_t last_checked = 0;       /* last integer examined             */

static inline void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    seq_t *new_ptr = mem_resize(seq_t, cache, new_cap);
    if (!new_ptr) exit(EXIT_FAILURE); /* abort on allocation failure */
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure cache is large enough to contain the n-th term.  */
    while (cached_len < (size_t)n) {
        seq_t candidate = last_checked + 1;
        last_checked = candidate;

        int pal_count = 0;
        for (idx_t base = 2; base <= 10 && pal_count < 2; ++base) {
            if (is_palindrome_base(candidate, base))
                ++pal_count;
        }
        if (pal_count >= 2) {
            if (cached_len == cache_cap)
                grow_cache();
            cache[cached_len++] = candidate;
        }
    }
    return cache[(size_t)(n - 1)];
}
