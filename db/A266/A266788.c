/*
 * A266788  Decimal representation of the n-th iteration of the "Rule 61"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula (OEIS A266788):
 *    a(0)=1, a(1)=3, a(2)=12, a(3)=87, a(4)=120, a(5)=1679, a(6)=976,
 *    a(7)=29823, a(8)=7808, a(9)=500735, a(10)=62464
 *    For n>10:  a(n) = 25*a(n-2) - 152*a(n-4) + 128*a(n-6)
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;     /* cache of computed terms */
static size_t cache_size = 0;       /* allocated capacity */
static idx_t last_computed = 0;     /* highest index computed */

/* Helper to grow the cache if needed */
static void ensure_cache(idx_t n)
{
    if (cache_size > (size_t)n)
        return;
    size_t new_size = cache_size < 16 ? 16 : cache_size;
    while (new_size <= (size_t)n)
        new_size <<= 1;
    seq_t *new_buf = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
    if (new_buf == NULL) { /* allocation failed – abort with zero */
        abort();
    }
    seq_cache = new_buf;
    /* initialize new slots to zero for safety */
    for (size_t i = cache_size; i < new_size; ++i)
        seq_cache[i] = (seq_t)0;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* Initialize cache with known first 11 terms */
    if (cache_size == 0) {
        ensure_cache(10);          /* capacity at least 11 (indices 0..10) */
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)3;
        seq_cache[2] = (seq_t)12;
        seq_cache[3] = (seq_t)87;
        seq_cache[4] = (seq_t)120;
        seq_cache[5] = (seq_t)1679;
        seq_cache[6] = (seq_t)976;
        seq_cache[7] = (seq_t)29823;
        seq_cache[8] = (seq_t)7808;
        seq_cache[9] = (seq_t)500735;
        seq_cache[10] = (seq_t)62464;
        last_computed = 10;
    }

    if (n <= last_computed)
        return seq_cache[(size_t)n];

    /* extend cache up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term = (seq_t)25 * seq_cache[(size_t)i - 2];
        seq_t sub  = (seq_t)152 * seq_cache[(size_t)i - 4];
        /* term >= sub for n > 10 per the recurrence description */
        term -= sub;
        term += (seq_t)128 * seq_cache[(size_t)i - 6];
        ensure_cache(i);
        seq_cache[(size_t)i] = term;
    }
    last_computed = n;
    return seq_cache[(size_t)n];
}
