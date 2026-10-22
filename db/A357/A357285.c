/*
 * A357285  a(n) = number of subsets S of {1,2,...,n} having more than 2
 *          elements such that (sum of least three elements of S) < max(S).
 * https://oeis.org/A357285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (n >= 8):
      a(n) = 4*a(n-1) - 3*a(n-2) - 2*a(n-3) - 6*a(n-4)
             + 8*a(n-5) + 24*a(n-6) - 40*a(n-7) + 16*a(n-8)
   Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0,
                    a(6)=0, a(7)=8
   Offset: 0   (a(0) is the first term) */
#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_cap = 0;  /* allocated size */
static idx_t seq_cache_max = 0;  /* highest index that has been computed */

static void ensure_cache_size(idx_t n)
{
    if (n + 1 <= seq_cache_cap)
        return;
    idx_t new_cap = seq_cache_cap ? seq_cache_cap << 1 : 16;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    /* initialise new entries to zero */
    for (idx_t i = seq_cache_cap; i < new_cap; ++i)
        seq_cache[i] = 0;
    seq_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 7)
        return 0;
    if (n == 7)
        return 8;

    ensure_cache_size(n);

    /* make sure the seed a(7) is initialised */
    if (seq_cache[7] == 0)
        seq_cache[7] = 8;

    /* compute from the highest already computed index */
    idx_t start = seq_cache_max + 1;
    if (start < 8)
        start = 8;

    for (idx_t i = start; i <= n; ++i)
    {
        seq_t pos = seq_cache[i - 1] * 4
                  + seq_cache[i - 5] * 8
                  + seq_cache[i - 6] * 24
                  + seq_cache[i - 8] * 16;
        seq_t neg = seq_cache[i - 2] * 3
                  + seq_cache[i - 3] * 2
                  + seq_cache[i - 4] * 6
                  + seq_cache[i - 7] * 40;
        seq_cache[i] = pos - neg;
    }

    seq_cache_max = n;
    return seq_cache[n];
}
