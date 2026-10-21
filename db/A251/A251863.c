/*
 * A251863  Numbers n such that the sum of the octagonal numbers N(n), N(n+1)
 *          and N(n+2) is equal to the sum of the pentagonal numbers P(m),
 *          P(m+1), P(m+2) for some m.
 * https://oeis.org/A251863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Path: A251863
   Formula: a(n) = a(n-1) + 1154*a(n-2) - 1154*a(n-3) - a(n-4) + a(n-5)
   a(1)=2, a(2)=147, a(3)=3102, a(4)=170407, a(5)=3580474
*/
#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache[1..max] holds a(1)...a(max) */
static idx_t seq_cache_max = 0;   /* maximum index computed so far */

static void ensure_cache(idx_t n)
{
    if (n <= seq_cache_max) return;

    idx_t new_size = n + 1; /* we want indices 0..n inclusive */
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* out of memory – abort (program will likely terminate) */
        abort();
    }
    seq_cache = tmp;

    /* pre‑define the first five terms – they are known constants */
    static const seq_t base[6] = {0, 2, 147, 3102, 170407, 3580474};
    for (idx_t i = 1; i <= 5 && i <= n; ++i)
        seq_cache[i] = base[i];

    /* compute subsequent terms using the recurrence */
    for (idx_t i = seq_cache_max + 1; i <= n; ++i) {
        if (i <= 5) continue;   /* already set */

        /* compute a[i] = a[i-1] + 1154*a[i-2] - 1154*a[i-3] - a[i-4] + a[i-5] */
        /* avoid intermediate underflow – differences are positive */
        seq_t diff23 = seq_cache[i-2] - seq_cache[i-3];          /* a[i-2] - a[i-3] */
        seq_t term23 = (seq_t)1154 * diff23;                    /* 1154*(a[i-2]-a[i-3]) */

        seq_t diff14  = seq_cache[i-1] - seq_cache[i-4];        /* a[i-1] - a[i-4] */
        seq_t term14p5 = diff14 + seq_cache[i-5];               /* a[i-1] - a[i-4] + a[i-5] */

        seq_cache[i] = term14p5 + term23;                       /* final value */
    }

    seq_cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* sequence offset is 1 */
    ensure_cache(n);
    return seq_cache[n];
}
