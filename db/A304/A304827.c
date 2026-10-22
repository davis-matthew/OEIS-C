/*
 * A304827  a(n) = 52*7^n/21 - 16/3 (n>=1).
 * https://oeis.org/A304827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=12, a(2)=116; for n>2, a(n)=8*a(n-1)-7*a(n-2).
   Equivalent closed form: a(n) = (52*7^n - 112)/21.  */
#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t cache_len  = 0;

static void ensure_cache(idx_t n)
{
    if (n <= cache_len)                 /* already computed up to n */
        return;

    idx_t old_len = cache_len;          /* how far we had computed */
    seq_cache = (seq_t *)realloc(seq_cache, (size_t)n * sizeof(seq_t));
    cache_len = n;                      /* we now have storage up to n */

    /* Initialise the first terms if we were empty */
    if (old_len == 0) {
        seq_cache[0] = (seq_t)12;       /* a(1) */
        old_len = 1;
        if (n >= 2) {
            seq_cache[1] = (seq_t)116;  /* a(2) */
            old_len = 2;
        }
    } else if (old_len == 1 && n >= 2) { /* we had only a(1) so far */
        seq_cache[1] = (seq_t)116;      /* a(2) */
        old_len = 2;
    }

    /* Compute any remaining terms up to n using the recurrence */
    for (idx_t i = old_len; i < n; ++i) {
        seq_cache[i] = 8 * seq_cache[i-1] - 7 * seq_cache[i-2];
    }
}

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1 */
    ensure_cache(n);
    return seq_cache[n-1];
}
