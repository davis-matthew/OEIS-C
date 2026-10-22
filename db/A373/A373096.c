/*
 * A373096  a(n) = a([n/9]) + a([n/27]) + a([n/81]) + ..., where a(0) = 0,
 *          a(1) = 1, and [ ] = floor().
 * https://oeis.org/A373096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a([n/9]) + a([n/27]) + a([n/81]) + …, * where a(0) = 0
 *            and a(1) = 1. * * The denominators are all powers of 3 ≥ 3²: 9,
 *            27, 81, 243, … . * The implementation uses a growing cache to
 *            memoize previously * computed values; it works for any idx_t /
 *            seq_t combination * supported by the build env...
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache for memoised values. */
static seq_t *a_cache    = NULL;
static idx_t  a_last     = 0;     /* highest index computed so far   */
static size_t a_cap      = 0;     /* current capacity of a_cache     */

/* Ensure that a_cache can hold indices up to n (inclusive). */
static void a_ensure_cap(idx_t n)
{
    if (n + 1 <= a_cap)
        return;
    size_t new_cap = a_cap ? a_cap << 1 : 2;
    while (new_cap <= (size_t)(n + 1))
        new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

/* Compute all terms up to index n, storing them in a_cache. */
static void a_compute_up_to(idx_t n)
{
    /* First time: allocate cache and initialise the base values. */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc(2 * sizeof(seq_t));
        a_cache[0] = (seq_t)0;
        a_cache[1] = (seq_t)1;
        a_cap   = 2;
        a_last  = 1;
    }

    if (n <= a_last)            /* already computed */
        return;

    a_ensure_cap(n);

    for (idx_t i = a_last + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t p = 9;                 /* first denominator: 3² */
        while (p <= i) {
            sum += a_cache[i / p];
            /* Prepare next denominator (multiply by 3).  Stop if overflow
             * or next value would exceed i. */
            if (p > i / 3)
                break;
            p *= 3;
        }
        a_cache[i] = sum;
    }
    a_last = n;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    a_compute_up_to(n);
    return a_cache[n];
}
