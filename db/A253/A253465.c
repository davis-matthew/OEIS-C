/*
 * A253465  Number of (n+1) X (4+1) 0..2 arrays with every 2 X 2 subblock
 *          diagonal maximum minus antidiagonal minimum unequal to its
 *          neighbors horizontally, vertically, diagonally and antidiagonally.
 * https://oeis.org/A253465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 16*a(n-4) - 16*a(n-6) for n > 8 with a(1)=24208,
 *            a(2)=3072, a(3)=2600, a(4)=3744, a(5)=7496, a(6)=8272,
 *            a(7)=15680, a(8)=31824, a(9)=94016, … (OEIS A253465)
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t max_n_comp = 0;          /* highest n that has been computed */

    /* base values (index 0 unused) */
    static const seq_t base[] = {
        0,          /* base[0] dummy                      */
        24208,      /* a(1) */
        3072,       /* a(2) */
        2600,       /* a(3) */
        3744,       /* a(4) */
        7496,       /* a(5) */
        8272,       /* a(6) */
        15680,      /* a(7) */
        31824,      /* a(8) */
        94016       /* a(9) not needed for recurrence */
    };

    /* ensure enough space for n+1 elements (index 0..n) */
    size_t needed = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
    } else if ((size_t)max_n_comp + 1 < needed) {
        cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
    }
    if (cache == NULL) {          /* allocation failed – return 0 */
        return 0;
    }

    /* initialise base values if nothing has been computed yet */
    if (max_n_comp == 0) {
        size_t upto = n < 8 ? n : 8;
        for (size_t i = 1; i <= upto; ++i) {
            cache[i] = base[i];
        }
        max_n_comp = (idx_t)upto;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_n_comp + 1; i <= n; ++i) {
        if (i <= 8) {                      /* already set for i<=8 */
            cache[i] = base[i];
            continue;
        }
        /* recurrence: a(i) = a(i-2) + 16*a(i-4) - 16*a(i-6) */
        seq_t term = cache[i - 2];
        term += cache[i - 4] * (seq_t)16;
        term -= cache[i - 6] * (seq_t)16;
        cache[i] = term;
    }

    max_n_comp = n;
    return cache[n];
}
