/*
 * A302511  Number of nX4 0..1 arrays with every element equal to 0, 1, 4 or 5
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=5, a(2)=5, a(3)=15, a(4)=28, a(5)=48, a(6)=89, a(7)=163,
   a(8)=297, a(9)=544, a(10)=1004,
   a(n)=a(n-1)+2*a(n-3)+2*a(n-4)-a(n-6)-a(n-7)   for n>=11   */
#include <stdlib.h>
#include <stddef.h>   /* for size_t */

static seq_t *cache = (seq_t *)NULL;    /* array holding a(1)..a(k) */
static size_t capacity = 0;              /* allocated size of the array     */
static size_t stored = 0;                /* number of values already filled */

/* Allocate or grow the cached array so that it can hold indices up to n (1‑based) */
static void ensure_capacity(idx_t n)
{
    if (capacity >= (size_t)n) return;

    size_t new_cap = capacity ? capacity : 16;
    while (new_cap < (size_t)n) new_cap <<= 1;   /* double until large enough */

    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) return;                            /* allocation failed – caller will handle */

    if (cache) {
        for (size_t i = 0; i < capacity; ++i) tmp[i] = cache[i];
        free(cache);
    }
    cache = tmp;
    capacity = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for 0‑based call */

    /* Ensure the cache is large enough and fill missing entries */
    ensure_capacity(n);

    /* Initial values (n = 1..10) */
    static const seq_t init[10] = {
        (seq_t)5,  (seq_t)5,  (seq_t)15, (seq_t)28,
        (seq_t)48, (seq_t)89, (seq_t)163, (seq_t)297,
        (seq_t)544, (seq_t)1004
    };

    /* Compute any missing terms */
    for (idx_t i = stored + 1; i <= n; ++i) {
        size_t idx = (size_t)i - 1;          /* array index for a(i) */
        if (i <= 10) {
            cache[idx] = init[idx];
        } else {
            /* recurrence: a(i)=a(i-1)+2*a(i-3)+2*a(i-4)-a(i-6)-a(i-7) */
            seq_t val = cache[idx - 1]             /* a(i-1) */
                        + 2 * cache[idx - 3]         /* 2*a(i-3) */
                        + 2 * cache[idx - 4]         /* 2*a(i-4) */
                        - cache[idx - 6]             /* -a(i-6) */
                        - cache[idx - 7];            /* -a(i-7) */
            cache[idx] = val;
        }
    }
    stored = (size_t)n;
    return cache[(size_t)n - 1];
}
