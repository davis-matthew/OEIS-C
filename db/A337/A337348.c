/*
 * A337348  Numbers formed as the product of two numbers without consecutive
 *          equal binary digits and sharing no common bits between them.
 * https://oeis.org/A337348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A000975(n-1) * A000975(n) Recurrence: a(n) = 5*a(n-1) -
 *            20*a(n-3) + 16*a(n-4), for n>4 Initial values: a(1)=0, a(2)=2,
 *            a(3)=10, a(4)=50
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_size   = 0;
static idx_t  max_computed = 0;

/* Ensure cache can hold indices up to n (inclusive) and
   initialize the base cases a(1)..a(4). */
static void ensure_capacity(idx_t n)
{
    if (cache_size == 0 || (idx_t)(n + 1) > cache_size) {
        size_t new_size = (cache_size > 0) ? cache_size * 2 : 64;
        while ((size_t)(n + 1) > new_size) new_size <<= 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    if (max_computed < 4) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)50;
        max_computed = 4;
    }
}

seq_t a(idx_t n)
{
    /* a(1) is the first term; behavior for n<1 is irrelevant */
    if (n < 1) return (seq_t)0;

    ensure_capacity(n);

    if (n <= max_computed)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val;
        if (i >= 5) {
            /* recurrence: 5*a(i-1) - 20*a(i-3) + 16*a(i-4) */
            val = (seq_t)(5 * cache[i-1] - 20 * cache[i-3] + 16 * cache[i-4]);
        } else {      /* these cases happen only on the very first call */
            switch (i) {
            case 1: val = (seq_t)0;  break;
            case 2: val = (seq_t)2;  break;
            case 3: val = (seq_t)10; break;
            case 4: val = (seq_t)50; break;
            default: val = 0;        /* unreachable */
            }
        }
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
