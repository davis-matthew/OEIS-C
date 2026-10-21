/*
 * A287896  a(n) = A002487(n)*A001511(n).
 * https://oeis.org/A287896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = (fusc(n-1)+fusc(n)+fusc(n+1))/2,
 *            where fusc is Stern's diatomic series
 * Range:     u8 n <= 2645, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static cache for fusc values (indexed by idx_t). */
static seq_t *fusc_cache = NULL;
static idx_t fusc_cap = 0;

/* Ensure the cache can address indices up to n.  */
static void fusc_ensure(idx_t n)
{
    if (n < fusc_cap)
        return;

    idx_t new_cap = fusc_cap ? fusc_cap << 1 : 1;
    while (new_cap <= n)
        new_cap <<= 1;

    fusc_cache = (seq_t *)realloc(fusc_cache, new_cap * sizeof(seq_t));
    if (new_cap > fusc_cap)
        memset(fusc_cache + fusc_cap, 0,
               (new_cap - fusc_cap) * sizeof(seq_t));

    fusc_cap = new_cap;
}

/* Fold recursion for Stern's diatomic series with memoisation. */
static seq_t fusc(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    fusc_ensure(n);

    seq_t cached = fusc_cache[n];
    if (cached != 0)
        return cached;

    seq_t value;
    if ((n & 1) == 0)          /* even */
        value = fusc(n >> 1);
    else                       /* odd */
        value = fusc(n >> 1) + fusc((n >> 1) + 1);

    fusc_cache[n] = value;
    return value;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;            /* a(1) = 1 */

    seq_t f_prev = fusc(n - 1);
    seq_t f_curr = fusc(n);
    seq_t f_next = fusc(n + 1);

    return (f_prev + f_curr + f_next) / 2;
}
