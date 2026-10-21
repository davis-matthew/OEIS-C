/*
 * A287826  Number of sequences over the alphabet {0,1,...,9} such that no two
 *          consecutive terms have distance 2.
 * https://oeis.org/A287826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 13*a(n-2) - 2*a(n-3), a(0)=1, a(1)=10,
 *            a(2)=84
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *a_cache = NULL;
static idx_t  a_cache_size = 0;   /* number of values stored */
static size_t a_cache_cap   = 0;   /* allocated capacity */

static inline seq_t mul_const(seq_t v, unsigned int m)
{
#if defined(__SIZEOF_INT128__) && !defined(_WIN32)
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 tmp = (unsigned __int128)v * m;
        return (seq_t)tmp;
    }
#endif
    return v * (seq_t)m;
}

static void ensure_a_cache(idx_t n)
{
    /* Ensure capacity >= n+1 */
    if (a_cache_cap > 0 && (idx_t)(a_cache_cap - 1) >= n) return;
    size_t new_cap = a_cache_cap ? a_cache_cap : 1;
    while (new_cap - 1 <= (size_t)n) new_cap <<= 1;
    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    if (a_cache && a_cache_size > 0) {
        memcpy(new_arr, a_cache, a_cache_size * sizeof(seq_t));
        free(a_cache);
    }
    a_cache   = new_arr;
    a_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 3) {
        switch ((int)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)10;
            case 2: return (seq_t)84;
        }
    }

    if (a_cache_size == 0) {
        ensure_a_cache(2);          /* capacity >= 3 */
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)10;
        a_cache[2] = (seq_t)84;
        a_cache_size = 3;
    }

    if (n < a_cache_size) return a_cache[n];

    ensure_a_cache(n);              /* capacity >= n+1 */

    for (idx_t i = a_cache_size; i <= n; ++i) {
        seq_t t = mul_const(a_cache[i-1], 10);
        t -= mul_const(a_cache[i-2], 13);
        t -= mul_const(a_cache[i-3], 2);
        a_cache[i] = t;
    }
    a_cache_size = n + 1;
    return a_cache[n];
}
