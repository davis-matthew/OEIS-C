/*
 * A317592  Lexicographically first sequence of different terms such that
 *          erasing the last two digits of a(n+1) and adding this new reshaped
 *          integer to a(n) gives back a(n+1).
 * https://oeis.org/A317592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=99, for n≥1 let k=a(n); a(n+1)= 100*⌊k/99⌋ + (k mod 99) if
 *            k mod 99≠0 or ⌊k/99⌋=1 100*⌊k/99⌋ – 1 if k mod 99=0 and ⌊k/99⌋>1
 *            The sequence is strictly increasing, so each term is
 *            automatically distinct
 * Range:     u8 n <= 129, u16 n <= 704, u32 n <= 1545, u64 n <= 1545,
 *            u128 n <= 1545, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* undefined offset handled gracefully */
    if (n == 0)
        return (seq_t)0;

    /* static cache for memoization */
    static seq_t *cache = NULL;   /* cache[0] = a(1), cache[1] = a(2), ... */
    static idx_t cap   = 0;       /* allocated capacity (in seq_t terms) */
    static idx_t max   = 0;       /* largest n computed so far (1‑based) */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 16;                                /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)99;                    /* a(1) */
        max = 1;
    }

    /* grow cache if necessary */
    if (n > cap) {
        size_t newcap = cap;
        while (newcap < n)
            newcap <<= 1;                       /* double until enough */
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = (idx_t)newcap;
    }

    /* compute missing terms up to n */
    if (n > max) {
        for (idx_t i = max + 1; i <= n; ++i) {
            seq_t prev = cache[i - 2];          /* a(i-1)   */
            seq_t q    = prev / (seq_t)99;      /* ⌊prev/99⌋ */
            seq_t rem  = prev % (seq_t)99;      /* prev mod 99 */
            seq_t next;

            if (rem == 0 && q > (seq_t)1)       /* multiple of 99, q>1 */
                next = ((seq_t)100 * q) - (seq_t)1;
            else
                next = ((seq_t)100 * q) + rem;   /* normal case */

            cache[i - 1] = next;                 /* store a(i)   */
        }
        max = n;
    }

    return cache[n - 1];
}
