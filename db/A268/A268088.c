/*
 * A268088  Number of nX3 0..2 arrays with every repeated value in every row
 *          not one larger and in every column one larger mod 3 than the
 *          previous repeated value, and upper left element zero.
 * https://oeis.org/A268088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Empirical linear recurrence of order 10:
 *   a(n) = 27*a(n-1) - 88*a(n-2) - 2949*a(n-3) + 19763*a(n-4) + 105684*a(n-5)
 *         - 917208*a(n-6) - 1283256*a(n-7) + 14673312*a(n-8) + 279936*a(n-9)
 *         - 60466176*a(n-10)
 *
 * Initial values (n = 1 … 10):
 *   9, 243, 4608, 83349, 1339893, 20699712,
 *   303284403, 4320438183, 59802048000, 811951057125
 *
 * The function returns a(n) for any n ≥ 1.  The implementation
 * memoizes results in a dynamic array so that repeated calls are fast,
 * and it works for any build where seq_t supports the usual arithmetic
 * operators.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Return the n-th term (n ≥ 1).  For n = 0 the function returns 0. */
seq_t a(idx_t n)
{
    /* Handle the unused n == 0 case */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Dynamic memoisation cache */
    static seq_t *cache = NULL;      /* cache[0] holds a(1) */
    static idx_t len   = 0;          /* number of terms currently stored */
    static idx_t cap   = 0;          /* allocated capacity of cache */

    /* Resize cache if necessary */
    if (n > cap) {
        idx_t new_cap = n;          /* allocate exactly n entries */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                 /* allocation failure – clean up */
            if (cache) free(cache);
            cache = NULL;
            len = cap = 0;
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initial terms (first 10 values of the sequence) */
    /* These are only seeds for the linear recurrence. */
    static const seq_t init[10] = {
        (seq_t)9,
        (seq_t)243,
        (seq_t)4608,
        (seq_t)83349,
        (seq_t)1339893,
        (seq_t)20699712,
        (seq_t)303284403,
        (seq_t)4320438183,
        (seq_t)59802048000,
        (seq_t)811951057125
    };

    /* Compute terms up to n, if not already cached */
    for (idx_t i = len; i < n; ++i) {          /* i is zero‑based: cache[i] == a(i+1) */
        if (i < (idx_t)10) {                   /* fill from the seed table   */
            cache[i] = init[i];
            continue;
        }

        /* Linear recurrence of order 10 */
        seq_t v = (seq_t)27  * cache[i - 1];
        v -= (seq_t)88  * cache[i - 2];
        v -= (seq_t)2949 * cache[i - 3];
        v += (seq_t)19763 * cache[i - 4];
        v += (seq_t)105684 * cache[i - 5];
        v -= (seq_t)917208 * cache[i - 6];
        v -= (seq_t)1283256 * cache[i - 7];
        v += (seq_t)14673312 * cache[i - 8];
        v += (seq_t)279936 * cache[i - 9];
        v -= (seq_t)60466176 * cache[i -10];

        cache[i] = v;
    }

    len = n;                                 /* all terms up to n are cached */
    return cache[n - 1];
}
