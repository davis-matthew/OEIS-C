/*
 * A306967  a(n) is the first Zagreb index of the Fibonacci cube Gamma(n).
 * https://oeis.org/A306967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-3) + 3*a(n-5) + a(n-6) for n > 6 with
 *            initial values a(1) = 2, a(2) = 6, a(3) = 22, a(4) = 54, a(5) =
 *            132, a(6) = 292
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence; 1‑based indices are stored from position 0. */
static seq_t *seq_cache = NULL;
static idx_t  cache_cap   = 0;

/* Ensure that seq_cache holds at least n terms (n >= 1). */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    size_t old_cap = cache_cap;
    size_t new_cap = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* allocation failure – terminate program */
    seq_cache = tmp;

    /* Fill newly added entries. */
    for (idx_t i = old_cap; i < n; ++i) {
        idx_t idx = i + 1;       /* 1‑based index of the term */

        switch (idx) {
            case 1: seq_cache[i] = (seq_t)2;   break;
            case 2: seq_cache[i] = (seq_t)6;   break;
            case 3: seq_cache[i] = (seq_t)22;  break;
            case 4: seq_cache[i] = (seq_t)54;  break;
            case 5: seq_cache[i] = (seq_t)132; break;
            case 6: seq_cache[i] = (seq_t)292; break;
            default:
                /* For n > 6 use the recurrence:
                   a(n) = 3*a(n-1) - 5*a(n-3) + 3*a(n-5) + a(n-6) */
                seq_t a1 = seq_cache[i-1];
                seq_t a3 = seq_cache[i-3];
                seq_t a5 = seq_cache[i-5];
                seq_t a6 = seq_cache[i-6];
                seq_cache[i] = a1*(seq_t)3
                               - a3*(seq_t)5
                               + a5*(seq_t)3
                               + a6;
                break;
        }
    }

    cache_cap = new_cap;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* defined only for n >= 1 */

    ensure_cache(n);
    return seq_cache[n-1];
}
