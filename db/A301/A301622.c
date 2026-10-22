/*
 * A301622  Numbers not divisible by 2, 3 or 5 (A007775) with digital root 4.
 * https://oeis.org/A301622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=13, a(2)=31, a(3)=49, a(4)=67, a(5)=103;
   a(n) = a(n-1) + a(n-4) - a(n-5) for n > 5.  */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t cached_max = 0;

static void ensure_cap(size_t needed)
{
    if (needed <= cap) return;
    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocations are expected to succeed */
    cache = tmp;
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but safe fallback */
    size_t sz = (size_t)n;
    ensure_cap(sz + 1);                  /* +1 to allow 1‑based indexing */

    if (n <= cached_max) return cache[(size_t)n];

    /* initialize first 5 terms if not already done */
    if (cached_max < 5) {
        cache[1] = 13;
        if (sz >= 1) cache[1] = 13;
        if (sz >= 2) cache[2] = 31;
        if (sz >= 3) cache[3] = 49;
        if (sz >= 4) cache[4] = 67;
        if (sz >= 5) cache[5] = 103;
        cached_max = (cached_max < 5) ? 5 : cached_max;
    }

    /* compute terms up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i <= 5) {
            switch (i) {
                case 1: cache[1] = 13; break;
                case 2: cache[2] = 31; break;
                case 3: cache[3] = 49; break;
                case 4: cache[4] = 67; break;
                case 5: cache[5] = 103; break;
            }
        } else {
            cache[(size_t)i] =
                cache[(size_t)(i - 1)] +
                cache[(size_t)(i - 4)] -
                cache[(size_t)(i - 5)];
        }
    }
    cached_max = n;
    return cache[(size_t)n];
}
