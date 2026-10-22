/*
 * A305511  Number of nX3 0..1 arrays with every element unequal to 1, 2, 5, 6
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 4*a(n-3) + 3*a(n-4) - 4*a(n-6) - 5*a(n-7) -
 *            a(n-8) - 3*a(n-9) - 2*a(n-10), for n > 13 Initial values (a(1)
 *            to a(13)): a(1)=1, a(2)=2, a(3)=5, a(4)=6, a(5)=8, a(6)=19,
 *            a(7)=33, a(8)=60, a(9)=107, a(10)=204, a(11)=375, a(12)=677,
 *            a(13)=1253
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* cache[0] unused, cache[i] holds a(i) */
static size_t cache_len = 0;  /* highest index computed (0 if none) */
static size_t cache_cap = 0;  /* allocated capacity */

/* Ensure cache can hold indices up to "needed". */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *newptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!newptr) abort(); /* unrecoverable allocation failure */
    cache = newptr;
    cache_cap = new_cap;
}

/* Compute a(n) for any n >= 1.  Subsequent calls cache values for reuse. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1; return 0 as safe fallback */

    /* Expand cache if needed */
    if ((size_t)n >= cache_len) {
        size_t need = (size_t)n + 1; /* index n is needed */
        ensure_cache_size(need);
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 1) cache[i] = (seq_t)1;
            else if (i == 2) cache[i] = (seq_t)2;
            else if (i == 3) cache[i] = (seq_t)5;
            else if (i == 4) cache[i] = (seq_t)6;
            else if (i == 5) cache[i] = (seq_t)8;
            else if (i == 6) cache[i] = (seq_t)19;
            else if (i == 7) cache[i] = (seq_t)33;
            else if (i == 8) cache[i] = (seq_t)60;
            else if (i == 9) cache[i] = (seq_t)107;
            else if (i == 10) cache[i] = (seq_t)204;
            else if (i == 11) cache[i] = (seq_t)375;
            else if (i == 12) cache[i] = (seq_t)677;
            else if (i == 13) cache[i] = (seq_t)1253;
            else {
                /* Recurrence for i > 13: express as pos - neg to avoid unsigned underflow */
                seq_t pos = cache[i-2];
                pos += 4 * cache[i-3];
                pos += 3 * cache[i-4];
                seq_t neg = 4 * cache[i-6];
                neg += 5 * cache[i-7];
                neg += cache[i-8];
                neg += 3 * cache[i-9];
                neg += 2 * cache[i-10];
                cache[i] = pos - neg;
            }
        }
        cache_len = need;
    }
    return cache[(size_t)n];
}
