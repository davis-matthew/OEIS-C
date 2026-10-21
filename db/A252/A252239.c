/*
 * A252239  Number of (n+2) X (4+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not equal to 1 3 4 6 or 7 and every 3 X 3 diagonal
 *          and antidiagonal sum equal to 1 3 4 6 or 7.
 * https://oeis.org/A252239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 260*a(n-3) - 1024*a(n-6) for n > 7 Initial values (n = 1
 *            … 7): a(1) = 4708 a(2) = 43848 a(3) = 331884 a(4) = 1179792 a(5)
 *            = 11206944 a(6) = 84935088 a(7) = 301990464 The function keeps a
 *            cache of computed terms and can be called in any order of n
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


static seq_t *cache   = NULL;   /* cache[0] holds a(1) */
static size_t cache_cap = 0;    /* allocated size */
static size_t cache_len = 0;    /* number of terms currently stored */

static const seq_t init_vals[7] = {
    (seq_t)4708ULL,
    (seq_t)43848ULL,
    (seq_t)331884ULL,
    (seq_t)1179792ULL,
    (seq_t)11206944ULL,
    (seq_t)84935088ULL,
    (seq_t)301990464ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, return 0 */

    /* Fast path for the first seven terms */
    if (n <= 7) return init_vals[(size_t)n - 1];

    /* Ensure cache can hold at least n terms */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        if (new_cap < 8) new_cap = 8;   /* keep space for the 7 init terms */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first seven terms on first use */
    if (cache_len < 7) {
        for (size_t i = 0; i < 7; ++i) cache[i] = init_vals[i];
        cache_len = 7;
    }

    /* Return already computed value if possible */
    if ((size_t)n <= cache_len) return cache[(size_t)n - 1];

    /* Compute missing terms up to n using the recurrence */
    /* cache[i] holds a(i+1); we need a(i-2) and a(i-5) for recurrence:
       a(i+1) = 260*a(i-2) - 1024*a(i-5)  → zero‑based indices i-3 and i-6 */
    size_t N = (size_t)n;
    for (size_t i = cache_len; i < N; ++i) {
        cache[i] = 260 * cache[i - 3] - 1024 * cache[i - 6];
    }
    cache_len = N;
    return cache[N - 1];
}
