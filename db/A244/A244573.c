/*
 * A244573  Numbers n such that 10*n + d - digsum(10*n + d) is a palindrome
 *          for any d in {0,1,2,3,4,5,6,7,8,9}.
 * https://oeis.org/A244573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 101, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, ensure_capacity, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244573.  a(n) is the n‑th positive integer k such that
   for every d in {0,…,9} the number 10*k + d – digsum(10*k + d)
   is a palindrome.  The function generates the sequence on demand
   and memoises the results. */

#include <stdbool.h>
#include <stddef.h>

#include "../utils/ensure_capacity.h"
#include "../utils/digit_sum.h"
#include "../utils/is_palindrome.h"

static seq_t *cache = NULL;          /* memoised terms */
static size_t cache_cap = 0;        /* current allocation size */
static size_t cache_len = 0;        /* number of terms stored */
static seq_t last_checked = 0;      /* largest integer examined so far */

/* Return true if k satisfies the definition of the sequence. */
static bool good_n(seq_t k)
{
    for (int d = 0; d <= 9; ++d) {
        seq_t t  = k * 10 + d;          /* 10*k + d */
        seq_t ds = digit_sum(t);        /* digsum(10*k + d) */
        seq_t m  = t - ds;              /* the candidate palindrome */
        if (!is_palindrome(m, 10))
            return false;
    }
    return true;
}

/* Generate terms until we have at least n terms.  The result is stored
   in the static array 'cache'.  This function is thread‑unsafe but the
   specifications never require concurrent access. */
static void generate_up_to(size_t n)
{
    while (cache_len < n) {
        seq_t cand = last_checked + 1;      /* next integer to test */

        if (good_n(cand)) {                 /* satisfies the condition?   */
            if (cache_len == cache_cap) {    /* ensure space                */
                /* increase capacity by one or double old capacity */
                size_t new_cap = cache_cap ? 2 * cache_cap : 16;
                if (cache_len + 1 > new_cap)
                    new_cap = cache_len + 1;
                ensure_capacity(new_cap, &cache_cap,
                                 (void **)&cache, sizeof(seq_t));
            }
            cache[cache_len++] = cand;        /* store term                */
        }
        last_checked = cand;                 /* advance candidate counter */
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n=0, but avoid segfault */

    /* Generate terms on demand */
    generate_up_to((size_t)n);

    return cache[(size_t)n - 1];
}
