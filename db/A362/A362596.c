/*
 * A362596  Number of parking functions of size n avoiding the patterns 213
 *          and 321.
 * https://oeis.org/A362596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n+1)*(n^2 + 3*n - 24)*a(n) = 2*(4*n^3 + 15*n^2 - 97*n +
 *            12)*a(n-1) - 8*(2*n - 3)*(n^2 + 5*n - 20)*a(n-2) with initial
 *            values a(0)=1, a(1)=1, a(2)=3, a(3)=13
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External integer types */
#ifndef SEQ_T_DEFINED
#define SEQ_T_DEFINED
#endif

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache   = NULL; /* array of cached terms */
static idx_t  cache_len = 0;     /* allocated length */
static idx_t  cache_used= 0;     /* highest computed index + 1 */

static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;
    size_t new_len = cache_len ? cache_len * 2 : 4;
    while (new_len <= (size_t)n) new_len *= 2;

    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache) abort();

    for (idx_t i = 0; i < cache_used; ++i) new_cache[i] = a_cache[i];
    for (idx_t i = cache_used; i < new_len; ++i) new_cache[i] = 0;

    free(a_cache);
    a_cache = new_cache;
    cache_len = new_len;
}

static void init_cache(void)
{
    if (cache_used == 0) {
        ensure_cache(3);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)3;
        a_cache[3] = (seq_t)13;
        cache_used = 4;
    }
}

seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_used) return a_cache[n];

    ensure_cache(n);

    for (idx_t i = cache_used; i <= n; ++i)
    {
        seq_t a_prev      = a_cache[i - 1];
        seq_t a_prev_prev = a_cache[i - 2];

        if (sizeof(seq_t) <= 8) {
            /* All arithmetic in unsigned __int128 to avoid overflow
               for the small integer types (uint8_t … uint64_t).   */
            unsigned __int128 k   = (unsigned __int128)i;
            unsigned __int128 N   = (k + 1) * (k * k + 3 * k - 24);
            unsigned __int128 poly1 = 4 * k * k * k + 15 * k * k - 97 * k + 12;
            unsigned __int128 poly2 = (2 * k - 3) * (k * k + 5 * k - 20);
            unsigned __int128 term1 = 2 * poly1 * (unsigned __int128)a_prev;
            unsigned __int128 term2 = 8 * poly2 * (unsigned __int128)a_prev_prev;
            unsigned __int128 numer = term1 - term2;
            a_cache[i] = (seq_t)(numer / N);
        } else {
            /* seq_t is larger (e.g. 128‑bit or big‑int); use its own
               arithmetic.  All intermediate integer multiplications
               are performed in 64‑bit unsigned to keep the code
               portable.                                            */
            seq_t N = (seq_t)(i + 1) * (seq_t)(i * i + 3 * (seq_t)i - 24);

            seq_t poly1 = (seq_t)(4 * (unsigned long long)i * i * i
                                  + 15 * (unsigned long long)i * i
                                  - 97 * (unsigned long long)i
                                  + 12);

            seq_t poly2 = (seq_t)((2 * (seq_t)i - 3)
                                  * (seq_t)(i * i + 5 * (seq_t)i - 20));

            seq_t term1 = (seq_t)2 * poly1 * a_prev;
            seq_t term2 = (seq_t)8 * poly2 * a_prev_prev;

            seq_t numer = term1 - term2;
            a_cache[i] = numer / N;
        }
    }

    cache_used = n + 1;   /* we now have terms 0 … n */
    return a_cache[n];
}
