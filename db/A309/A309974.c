/*
 * A309974  a(1) = a(2) = a(3) = 1; a(n) = round((2*n-a(n-1)-a(n-2))/a(n-3))
 *          for n > 3.
 * https://oeis.org/A309974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=a(2)=a(3)=1;
   a(n)=round((2*n - a(n-1)-a(n-2))/a(n-3)) for n>3 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;
static idx_t      seq_max   = 0;
static size_t     seq_cap   = 0;

/* Ensure the cache can hold at least N+1 entries (index 0 unused). */
static void ensure_cache_size(idx_t N)
{
    while (seq_cap <= (size_t)N) {
        size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);          /* insufficient memory */
        seq_cache = new_arr;
        seq_cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n<1 */

    /* Allocate and seed initial terms on first invocation */
    if (seq_cache == NULL) {
        ensure_cache_size(3);
        seq_cache[0] = 0;                    /* unused */
        seq_cache[1] = 1;                    /* a(1) */
        seq_cache[2] = 1;                    /* a(2) */
        seq_cache[3] = 1;                    /* a(3) */
        seq_max = 3;
    }

    /* Extend cache up to required index */
    if (n > (idx_t)seq_max) {
        idx_t i;
        for (i = seq_max + 1; i <= n; ++i) {
            ensure_cache_size((size_t)i);
            seq_t prev1 = seq_cache[i - 1];
            seq_t prev2 = seq_cache[i - 2];
            seq_t prev3 = seq_cache[i - 3];

            seq_t numerator = ((seq_t)2 * i) - prev1 - prev2;
            seq_t div = prev3;

            /* Round to nearest integer: floor((num/ div) + 0.5) */
            seq_t value = (numerator + div / 2) / div;
            seq_cache[i] = value;
        }
        seq_max = n;
    }
    return seq_cache[n];
}
