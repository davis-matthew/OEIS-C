/*
 * A295675  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 1, a(1) = 1, a(2) =
 *          2, a(3) = -2.
 * https://oeis.org/A295675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 26, i32 n <= 49, i64 n <= 95, i128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A295675 – a(n) = a(n-1) + a(n-3) + a(n-4),
    a(0) = 1, a(1) = 1, a(2) = 2, a(3) = -2
    (follows the OEIS formula) */

#include <stdlib.h>
#include <stdbool.h>

/* static memoisation tables – external linkage for a() only */
static seq_t *cache      = NULL;   /* values of a(i) */
static bool  *present   = NULL;    /* true ⇔ a(i) already computed */
static idx_t  cache_size = 0;      /* number of slots allocated */

/* ensure that the arrays can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;                    /* already large enough */

    idx_t new_size = (n + 1) * 2;    /* grow a bit to avoid many reallocs */
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    bool  *new_present = (bool*)realloc(present, new_size * sizeof(bool));

    if (!new_cache || !new_present)          /* allocation failure */
        abort();

    /* initialise new slots as not yet present */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_present[i] = false;

    cache     = new_cache;
    present   = new_present;
    cache_size = new_size;
}

/* compute a(n) with memoised recursion/iteration */
seq_t a(idx_t n)
{
    if (n < 4) {                         /* handle the base cases directly */
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)-2;
        }
    }

    ensure_cache(n);

    /* compute all values up to n in increasing order */
    for (idx_t i = 0; i <= n; ++i) {
        if (present[i])
            continue;                    /* already computed */

        if (i < 4) {                     /* base values, also for i==0..3 */
            switch (i) {
                case 0: cache[i] = (seq_t)1; break;
                case 1: cache[i] = (seq_t)1; break;
                case 2: cache[i] = (seq_t)2; break;
                case 3: cache[i] = (seq_t)-2; break;
            }
        } else {
            /* recurrence: a(i) = a(i-1) + a(i-3) + a(i-4) */
            cache[i] = cache[i-1] + cache[i-3] + cache[i-4];
        }
        present[i] = true;
    }

    return cache[n];
}
