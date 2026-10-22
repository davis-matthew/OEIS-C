/*
 * A396534  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2 + (x + 79^2)^2 = y^2.
 * https://oeis.org/A396534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: A396534
    Nonnegative values x of solutions (x, y) to x^2 + (x + 79^2)^2 = y^2.
    Recurrence: a(n) = 6*a(n-5) - a(n-10) + 12482   for n >= 11
    Initial values:
        a(1)=0,  a(2)=1580,  a(3)=3636,  a(4)=8319,
        a(5)=12719, a(6)=18723, a(7)=26939, a(8)=38199,
        a(9)=64736, a(10)=90060.
*/

#include <stdlib.h>

/* Cached initial values (indices 1..10). */
static const seq_t init_vals[10] = {
    (seq_t)0,   (seq_t)1580,  (seq_t)3636,  (seq_t)8319,
    (seq_t)12719,(seq_t)18723, (seq_t)26939, (seq_t)38199,
    (seq_t)64736,(seq_t)90060
};

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* defined only for n >= 1 */

    /* Static cache and bookkeeping */
    static seq_t *cache = NULL;    /* cache[0] unused */
    static idx_t last = (idx_t)0;  /* highest index computed so far */
    static size_t cap  = 0;        /* current allocated size */

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;        /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute new terms up to n */
    for (idx_t i = last + (last == (idx_t)0 ? (idx_t)1 : (idx_t)1); /* start at next index */
         i <= n;
         ++i)
    {
        if (i <= (idx_t)10) {
            /* Pre‑computed initial values */
            cache[i] = init_vals[i-1];
        } else {
            /* Recurrence: a(i) = 6*a(i-5) - a(i-10) + 12482 */
            cache[i] = ( (cache[i-5] * (seq_t)6)
                         - cache[i-10] )
                         + (seq_t)12482;
        }
    }
    last = n;   /* all indices up to n are now valid */

    return cache[n];
}
