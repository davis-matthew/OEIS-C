/*
 * A373408  Minimum of the n-th maximal antirun of squarefree numbers
 *          differing by more than one.
 * https://oeis.org/A373408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that n == 1 or both n and n-1 are squarefree.
   The list is the increasing sequence 1,2,3,6,7,11,14,15,22,23,...
   a(n) is the n-th element of this sequence.
   The implementation keeps a static cache and extends it on demand.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration of the function required by the build */
seq_t a(idx_t n);

/* ---------- Helper: test for squarefree --------------------------------- */
static bool is_squarefree(idx_t x)
{
    if (x <= 1)
        return true;               /* 0 and 1 are considered squarefree */

    idx_t tmp = x;
    for (idx_t d = 2; d * d <= tmp; ++d)
    {
        if (tmp % d == 0)
        {
            /* Found a prime factor d */
            tmp /= d;
            if (tmp % d == 0)            /* exponent >= 2 */
                return false;
            /* Remove all remaining copies of d */
            while (tmp % d == 0)
                tmp /= d;
        }
    }
    return true;
}

/* ---------- Cached sequence storage ------------------------------------- */
static seq_t *cached = NULL;      /* array storing found numbers */
static size_t cached_len  = 0;     /* number of terms stored */
static size_t cached_cap  = 0;     /* current capacity of the array */
static idx_t  checked_to = 1;      /* largest integer that has been examined */

/* Extend the cache until it contains at least 'need' elements. */
static void ensure_terms(idx_t need)
{
    if (cached_len == 0)
    {
        cached_cap = 16;
        cached = (seq_t *)realloc(cached, cached_cap * sizeof(seq_t));
        cached[0] = (seq_t)1;      /* a(1) = 1 */
        cached_len = 1;
        checked_to = 1;
    }

    /* Scan successive integers until the cache is large enough. */
    idx_t cur = checked_to;
    while (cached_len < (size_t)need)
    {
        ++cur;
        if (is_squarefree(cur) && is_squarefree(cur - 1))
        {
            if (cached_len == cached_cap)
            {
                cached_cap = cached_cap ? cached_cap * 2 : 16;
                cached = (seq_t *)realloc(cached, cached_cap * sizeof(seq_t));
            }
            cached[cached_len++] = (seq_t)cur;
        }
    }
    checked_to = cur;
}

/* ---------- Public function required by the build ----------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n == 0; return 0 as a safe default */
        return (seq_t)0;

    ensure_terms(n);
    return cached[n - 1]; /* array is 0‑based: a(1) at index 0 */
}
