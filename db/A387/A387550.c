/*
 * A387550  a(n) = (1/2) * Sum_{k=0..n} 2^k * binomial(2*k+2,2*n-2*k+1).
 * https://oeis.org/A387550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*a(n-1) + 8*a(n-3) - 4*a(n-4)  (n >= 4),
 *           a(0)=1, a(1)=4, a(2)=16, a(3)=72 .
 *
 * Computes the OEIS sequence A387550 with memoization.
 * Supports any seq_t/idx_t combination; assumes seq_t supports
 * the usual integral operations and realloc may be used.
 */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed values */
static size_t capacity = 0;   /* allocated capacity */
static size_t count = 0;      /* number of initialized elements */

/* Ensure the array can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (capacity >= need) return;

    size_t new_cap = capacity ? capacity : 8;
    while (new_cap < need) new_cap <<= 1;          /* double until enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    capacity = new_cap;
}

/* Return a(n) for the OEIS sequence A387550 */
seq_t a(idx_t n)
{
    /* Base values handled explicitly for efficiency */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)16;
    if (n == 3) return (seq_t)72;

    /* Compute up to n, memoizing as we go */
    ensure_capacity(n);
    if (count > n) return cache[n];  /* already computed */

    for (idx_t i = (idx_t)count; i <= n; ++i) {
        if (i == 0)           cache[0] = (seq_t)1;
        else if (i == 1)      cache[1] = (seq_t)4;
        else if (i == 2)      cache[2] = (seq_t)16;
        else if (i == 3)      cache[3] = (seq_t)72;
        else {
            /* Recurrence: a(i) = 4*a(i-1) + 8*a(i-3) - 4*a(i-4) */
            cache[i] = cache[i-1] * (seq_t)4
                       + cache[i-3] * (seq_t)8
                       - cache[i-4] * (seq_t)4;
        }
    }
    count = n + 1;
    return cache[n];
}
