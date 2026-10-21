/*
 * A275362  An eventually quasilinear solution to Hofstadter's Q recurrence.
 * https://oeis.org/A275362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) satisfies a(n) = 2*a(n-9) - a(n-18) for n > 29. For 1 ≤ n ≤
 *            29 the values are given by the quasilinear pattern: a(9k) = 3
 *            a(9k+1) = 9k - 9 a(9k+2) = 10k + 2 a(9k+3) = 9 a(9k+4) = 2
 *            a(9k+5) = 9k a(9k+6) = 9 a(9k+7) = 9k a(9k+8) = 10k + 10 with
 *            the exceptions: a(6)=7, a(7)=9, a(11)=2
 * Range:     i8 n <= 115, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached sequence values   */
static size_t cache_capacity = 0;    /* allocated size of cache   */
static idx_t cache_size = 0;         /* number of values computed */

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache_for(idx_t n)
{
    if (n <= 0)
        return;                      /* a(0) is always 0 */

    /* Allocate or grow buffer. */
    if ((size_t)n + 1 > cache_capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute any missing terms. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 0;                     /* a(0) = 0 by definition */
        } else if (i <= 29) {             /* small‑index pattern */
            if (i == 6) {
                val = 7;
            } else if (i == 7) {
                val = 9;
            } else if (i == 11) {
                val = 2;
            } else {
                idx_t k = i / 9;          /* quotient */
                int m = (int)(i % 9);     /* remainder 0…8 */
                switch (m) {
                    case 0: val = 3; break;
                    case 1: val = (seq_t)9 * k - 9; break;
                    case 2: val = (seq_t)10 * k + 2; break;
                    case 3: val = 9; break;
                    case 4: val = 2; break;
                    case 5: val = (seq_t)9 * k; break;
                    case 6: val = 9; break;
                    case 7: val = (seq_t)9 * k; break;
                    case 8: val = (seq_t)10 * k + 10; break;
                    default: val = 0; /* unreachable */
                }
            }
        } else {                            /* recurrence for n > 29 */
            /* cache[i‑9] and cache[i‑18] are guaranteed to exist. */
            seq_t a_minus9 = cache[i - 9];
            seq_t a_minus18 = cache[i - 18];
            val = 2 * a_minus9 - a_minus18;
        }
        cache[i] = val;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n <= 0)
        return 0;                  /* a(n) = 0 for n <= 0 */

    ensure_cache_for(n);
    return cache[n];
}
