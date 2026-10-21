/*
 * A267452  Total number of ON (black) cells after n iterations of the "Rule
 *          131" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 417, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A267452
 *  Total number of ON cells after n iterations of Rule 131
 *
 *  Recurrence (Conjecture by Colin Barker):
 *      a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 6, a(4) = 10, a(5) = 13,
 *      a(6) = 19, a(7) = 24
 *      a(n) = a(n-1) + a(n-3) - a(n-5) - a(n-7) + a(n-8)   for n ≥ 8
 */

#include <stdlib.h>
#include <stddef.h>

/* static cache for memoisation ------------------------------------------------ */
static seq_t *cache   = NULL;   /* array of computed a(i) values            */
static idx_t  cap    = 0;       /* allocated size (in seq_t elements)       */
static idx_t  len    = 0;       /* number of values stored                  */

/* Ensure that the cache has at least 'needed' slots available.                  */
static void ensure_capacity(idx_t needed)
{
    if (cap >= needed)
        return;

    idx_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;

    /* In C++ a void* returned by realloc is not implicitly convertible to
       seq_t*, so perform an explicit cast. This works for both C and C++.     */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp)
        abort();          /* memory exhaustion – stop execution              */

    cache = tmp;
    cap   = new_cap;
}

/* Initialise the first eight base values of the sequence.                       */
static void init_base(void)
{
    if (len > 0)      /* already initialised */
        return;

    ensure_capacity(8);

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)4;
    cache[3] = (seq_t)6;
    cache[4] = (seq_t)10;
    cache[5] = (seq_t)13;
    cache[6] = (seq_t)19;
    cache[7] = (seq_t)24;

    len = 8;
}

/* Return a(n) – total number of ON cells after n iterations.                  */
seq_t a(idx_t n)
{
    init_base();                     /* guarantees that base terms exist */

    if (n < len)                     /* value already computed?           */
        return cache[n];

    ensure_capacity(n + 1);           /* make sure cache big enough        */

    /* Compute missing values up to n, using the recurrence.                  */
    for (idx_t i = len; i <= n; ++i) {
        /* The recurrence requires terms up to a(i-8); they exist because
           i ≥ 8 when len > 8. For i < 8 the base values are already stored.  */
        cache[i] = cache[i - 1] + cache[i - 3]
                 - cache[i - 5] - cache[i - 7] + cache[i - 8];
    }

    len = n + 1;                     /* update number of stored terms     */

    return cache[n];
}
