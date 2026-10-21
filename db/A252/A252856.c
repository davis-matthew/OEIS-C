/*
 * A252856  Number of (n+2) X (3+2) 0..2 arrays with every consecutive three
 *          elements in every row, column, diagonal and antidiagonal having
 *          exactly two distinct values, and new values 0 upwards introduced
 *          in row major order.
 * https://oeis.org/A252856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 26, u64 n <= 59, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A252856
   OEIS recurrence: a(n) = 2*a(n-1) - a(n-5) + a(n-6) - 2*a(n-7) + a(n-8)
   for n > 10, with initial values
   a(1)=684 a(2)=738 a(3)=1197 a(4)=1962 a(5)=3582
   a(6)=6795 a(7)=12762 a(8)=24552 a(9)=47367 a(10)=91440 .
   This implementation memoises the values so any order of calls works. */

#include <stdlib.h>

static seq_t init_vals[11] = {
    (seq_t)0,     /* unused 0‑th index */
    (seq_t)684,   (seq_t)738,   (seq_t)1197,  (seq_t)1962,
    (seq_t)3582,  (seq_t)6795,  (seq_t)12762, (seq_t)24552,
    (seq_t)47367, (seq_t)91440
};

static seq_t *cache = NULL;     /* array of computed a(k), indexed by k */
static size_t cache_cap = 0;    /* capacity of cache array (max index it can hold) */
static size_t cache_max = 0;    /* largest n for which cache[n] has been computed */

/* ensure that cache can store indices up to 'need' (inclusive) */
static void ensure_capacity(idx_t need)
{
    if ((size_t)need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while ((size_t)need > new_cap)
        new_cap *= 2;

    /* realloc may return void*; cast to seq_t* for both C and C++ */
    seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort(); /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n < 1, but return 0 safely */

    /* if already computed, return immediately */
    if (n <= cache_max)
        return cache[(size_t)n];

    /* grow storage as needed */
    ensure_capacity(n);

    /* compute all missing terms up to n */
    for (size_t k = cache_max + 1; k <= (size_t)n; ++k) {
        if (k <= 10) {          /* initial values known */
            cache[k] = init_vals[k];
        } else {                /* recurrence */
            seq_t v = 2 * cache[k - 1];
            v -= cache[k - 5];
            v += cache[k - 6];
            v -= 2 * cache[k - 7];
            v += cache[k - 8];
            cache[k] = v;
        }
    }

    cache_max = n;
    return cache[(size_t)n];
}
