/*
 * A252252  Number of (n+2) X (6+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not equal to 0 3 5 6 or 7 and every 3 X 3 diagonal
 *          and antidiagonal sum equal to 0 3 5 6 or 7.
 * https://oeis.org/A252252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) - 3*a(n-4) - a(n-6) + a(n-7) for n>9
 *            Initial values: a(1)=616, a(2)=359, a(3)=318, a(4)=336,
 *            a(5)=366, a(6)=405, a(7)=453, a(8)=531, a(9)=633
 * Range:     u8 none, u16 n <= 25, u32 n <= 59, u64 n <= 128, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* table of computed values (1‑based) */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* highest index computed */

/* extend cached table to contain at least indices up to n */
static void ensure_cache(idx_t n)
{
    size_t N = (size_t)n;
    /* grow table if needed */
    if (cache_cap <= N) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 8;
        if (new_cap <= N) new_cap = N + 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* initial values for n = 1..9 */
    static const seq_t init_vals[9] = {
        616, 359, 318, 336, 366, 405, 453, 531, 633
    };
    while (cache_len < N && cache_len < 9) {
        cache_len++;
        cache[cache_len] = init_vals[cache_len - 1];
    }

    /* recurrence for n > 9 */
    if (cache_len < N) {
        size_t start = cache_len < 10 ? 10 : cache_len;
        for (size_t i = start; i <= N; ++i) {
            seq_t term = cache[i - 1];
            term += 3 * cache[i - 3];
            term += cache[i - 7];
            seq_t sub = 3 * cache[i - 4];
            sub += cache[i - 6];
            term -= sub;          /* LHS > RHS, so no underflow */
            cache[i] = term;
        }
        cache_len = N;
    }
}

/* External function requested by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return 0 for safety */
    ensure_cache(n);
    return cache[n];
}
