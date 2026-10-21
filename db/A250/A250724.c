/*
 * A250724  Number of (n+1) X (3+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 5*a(n-4) - 4*a(n-5) + a(n-6) with
 *            initial values: a(1)=50, a(2)=110, a(3)=208, a(4)=365, a(5)=600,
 *            a(6)=942
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

static seq_t *seq_cache = NULL;   /* cache[ n ] holds a(n)  (n>=0, cache[0] unused) */
static size_t  seq_cap   = 0;      /* allocated size (>= n+1) */
static idx_t   seq_max   = 0;      /* highest n computed and stored */

/* expand cache to hold at least indices up to n */
static void ensure_capacity(idx_t n)
{
    if (n < seq_cap) return;
    size_t newCap = seq_cap ? seq_cap * 2 : 16;
    while (newCap <= (size_t)n) newCap <<= 1;
    seq_t *p = (seq_t*)realloc(seq_cache, newCap * sizeof(seq_t));
    assert(p != NULL);               /* abort on out‑of‑memory */
    if (seq_cap == 0) {
        /* first allocation: zero all entries then set base values */
        for (size_t i = 0; i < newCap; ++i) p[i] = 0;
        p[1] = 50;   p[2] = 110;   p[3] = 208;   p[4] = 365;
        p[5] = 600;  p[6] = 942;
        seq_max = 6;
    } else {
        for (size_t i = seq_cap; i < newCap; ++i) p[i] = 0;
    }
    seq_cache = p;
    seq_cap   = newCap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* sequence is defined for n>=1 */

    if (seq_cache == NULL) {           /* first call – initialise cache   */
        ensure_capacity(n);
        /* seq_max already set to 6 in first allocation */
    } else {
        ensure_capacity(n);
    }

    if (seq_max < n) {
        /* compute values for all indices up to n sequentially */
        for (idx_t k = seq_max + 1; k <= n; ++k) {
            /* recurrence holds for k >= 7, but for k <= 6 we already
               have base values; the loop starts at 7 */
            if (k <= 6) continue;   /* base values already in cache */

            seq_t term = 0;
            term += seq_cache[k-1] * (seq_t)4;
            term -= seq_cache[k-2] * (seq_t)5;
            term += seq_cache[k-4] * (seq_t)5;
            term -= seq_cache[k-5] * (seq_t)4;
            term += seq_cache[k-6];
            seq_cache[k] = term;
        }
        seq_max = n;
    }
    return seq_cache[n];
}
