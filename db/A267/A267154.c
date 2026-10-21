/*
 * A267154  Decimal representation of the n-th iteration of the "Rule 107"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 4, a(2) = 11, a(3) = 110, a(4) = 119, a(5) =
 *            1978, a(6) = 475, a(7) = 32510, a(8) = 1799, a(9) = 523002,
 *            a(10) = 6939, a(11) = 8388350, a(12) = 1799. For n > 12: a(n) =
 *            16*a(n-2) + a(n-4) - 16*a(n-6)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;            /* dynamically grown array of terms   */
static size_t cache_cap = 0;           /* allocated capacity                 */
static idx_t cache_len  = 0;           /* number of terms currently stored   */

seq_t a(idx_t n)
{
    /* If already computed, return immediately */
    if (n < cache_len)
        return cache[n];

    /* Compute new required size */
    size_t needed = (size_t)n + 1;

    /* Allocate memory if first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
    }

    /* Grow the buffer if necessary */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initial 13 terms, kept in a static array */
    static const seq_t init[13] = {
        (seq_t)1,          /* a(0)  */
        (seq_t)4,          /* a(1)  */
        (seq_t)11,         /* a(2)  */
        (seq_t)110,        /* a(3)  */
        (seq_t)119,        /* a(4)  */
        (seq_t)1978,       /* a(5)  */
        (seq_t)475,        /* a(6)  */
        (seq_t)32510,      /* a(7)  */
        (seq_t)1799,       /* a(8)  */
        (seq_t)523002,     /* a(9)  */
        (seq_t)6939,       /* a(10) */
        (seq_t)8388350,    /* a(11) */
        (seq_t)1799        /* a(12) */
    };

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 12) {
            cache[i] = init[i];
        } else {
            /* a(i) = 16*a(i-2) + a(i-4) - 16*a(i-6) */
            seq_t t1 = cache[i-2] * (seq_t)16;
            seq_t t2 = cache[i-6] * (seq_t)16;
            cache[i] = t1 + cache[i-4] - t2;
        }
    }

    cache_len = n + 1;
    return cache[n];
}
