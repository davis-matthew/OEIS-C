/*
 * A355898  a(1) = a(2) = 1; a(n) = gcd(a(n-1), a(n-2)) + (a(n-1) +
 *          a(n-2))/gcd(a(n-1), a(n-2)).
 * https://oeis.org/A355898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 1; a(n) = gcd(a(n-1), a(n-2)) + (a(n-1) + a(n-2))
 *            / gcd(a(n-1), a(n-2)) (n ≥ 3)
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 121, u64 n <= 237,
 *            u128 n <= 443, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;   /* memoised values a(1), a(2), … */
static size_t cache_len = 0;  /* number of terms currently stored */
static size_t cache_cap = 0;  /* current allocated capacity */

/* Return the n‑th term of OEIS A355898.  Works for any idx_t n ≥ 1
   and any seq_t type, including arbitrary‑precision types. */
seq_t a(idx_t n)
{
    /* Handle the trailing indices directly – they are always 1. */
    if (n < 3)
        return (seq_t)1;

    /* Ensure we have at least 'n' slots allocated. */
    size_t needed = (size_t)n;
    if (needed < 2)
        needed = 2;               /* we must store a(1) and a(2) for safety */
    if (cache_cap < needed) {
        size_t new_cap = needed > 32 ? needed * 2 : needed + 32;
        /* realloc returns void* in C; cast it for C++ builds as well. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first two terms if we have just started. */
    if (cache_len < 2) {
        cache[0] = (seq_t)1;            /* a(1) */
        cache[1] = (seq_t)1;            /* a(2) */
        cache_len = 2;
    }

    /* Compute missing terms up to n. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        size_t idx = i - 1;                 /* zero‑based index of a(i)   */
        seq_t a1   = cache[idx - 1];         /* a(i-1)  */
        seq_t a2   = cache[idx - 2];         /* a(i-2)  */
        seq_t g    = gcd(a1, a2);
        seq_t val  = g + (a1 + a2) / g;
        cache[idx] = val;
    }

    cache_len = (size_t)n;                  /* we have stored up to a(n) */
    return cache[n - 1];
}
