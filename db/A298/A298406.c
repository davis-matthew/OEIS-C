/*
 * A298406  a(n) = 2*a(n-1) - a(n-3) + a(floor(n/2)) + a(floor(n/3)) + ... +
 *          a(floor(n/n)), where a(0) = 1, a(1) = 1, a(2) = 1.
 * https://oeis.org/A298406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 41, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A298406:
   a(0) = a(1) = a(2) = 1
   a(n) = 2*a(n-1) - a(n-3)
          + a(floor(n/2)) + a(floor(n/3)) + … + a(floor(n/n)) */

/* The function is memoised and can be called in any order. */
#include <stdlib.h>

static seq_t *cache = NULL;     /* table of computed a(n) */
static size_t  cap   = 0;       /* current capacity of cache   */
static idx_t   next  = 0;       /* next index that has not been computed */

/* Ensure the cache can hold values up to index n (inclusive). */
static void ensure_cap(idx_t n)
{
    if ((size_t)(n + 1) <= cap) return;           /* already big enough */

    size_t newcap = cap ? (cap << 1) : 4;          /* double until big enough */
    while (newcap <= (size_t)(n + 1)) newcap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) {                                  /* allocation failed – abort */
        abort();
    }
    cache = tmp;
    cap   = newcap;
}

/* Main function for OEIS sequence A298406 */
seq_t a(idx_t n)
{
    /* initialise the first three terms once */
    if (cache == NULL) {
        ensure_cap(2);                           /* need indices 0..2 */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        next     = 3;                            /* first uncomputed index */
    }

    /* if n is less than 3, answer is 1 */
    if (n < 3) return 1;

    /* grow cache if necessary */
    ensure_cap(n);

    /* compute missing terms up to n */
    for (idx_t i = next; i <= n; ++i) {
        /* compute the sum Σ_{k=2..i} a(floor(i/k)) efficiently */
        seq_t sum = 0;
        idx_t k   = 2;
        while (k <= i) {
            idx_t q     = i / k;          /* floor(i/k) */
            idx_t last  = i / q;          /* largest k with the same quotient q */
            /* add (last - k + 1) * a[q] */
            sum += (seq_t)(last - k + 1) * cache[q];
            k = last + 1;
        }

        /* recurrence: a(i) = 2*a(i-1) - a(i-3) + sum */
        seq_t val = 2 * cache[i - 1] - cache[i - 3] + sum;
        cache[i] = val;
    }
    next = n + 1;                    /* all up to n computed */

    return cache[n];
}
