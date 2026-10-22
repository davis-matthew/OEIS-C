/*
 * A390219  a(n) = Sum_{k=0..floor(4*n/7)} binomial(k+2,4*n-7*k).
 * https://oeis.org/A390219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 61, u64 n <= 121,
 *            u128 n <= 241, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-2) – 6*a(n-4) + 4*a(n-6) + a(n-7) – a(n-8)
   Initial values:
     a(0)=1, a(1)=0, a(2)=3, a(3)=0,
     a(4)=6, a(5)=0, a(6)=10, a(7)=1
   Any n ≥ 0 is obtained from this recurrence.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;          /* cache of computed values       */
    static idx_t  last = (idx_t)-1;    /* last index that has a value    */
    static size_t capa = 0;            /* allocated capacity of buf      */

    /* If the cache is already built and we have the answer, return it. */
    if (buf != NULL && n <= last)
        return buf[n];

    /* First call: create the cache and store the base values. */
    if (buf == NULL) {
        capa = 8; /* we know values for 0..7 */
        buf = (seq_t *)calloc(capa, sizeof(seq_t));
        if (buf == NULL)          /* allocation failure */
            return (seq_t)0;

        buf[0] = (seq_t)1;
        buf[1] = (seq_t)0;
        buf[2] = (seq_t)3;
        buf[3] = (seq_t)0;
        buf[4] = (seq_t)6;
        buf[5] = (seq_t)0;
        buf[6] = (seq_t)10;
        buf[7] = (seq_t)1;
        last = 7;
    }

    /* Expand the cache if necessary. */
    if ((idx_t)(capa) <= n) {
        size_t new_capa = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_capa * sizeof(seq_t));
        if (tmp == NULL)          /* allocation failure */
            return (seq_t)0;
        /* Zero any new slots so that later additions are safe. */
        for (idx_t i = capa; i < new_capa; ++i)
            tmp[i] = (seq_t)0;
        buf = tmp;
        capa = new_capa;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 7)                /* base values already set */
            continue;

        seq_t pos = (seq_t)4 * buf[i - 2]
                  + (seq_t)4 * buf[i - 6]
                  +       buf[i - 7];
        seq_t neg = (seq_t)6 * buf[i - 4]
                  +       buf[i - 8];
        buf[i] = pos - neg;        /* guaranteed non‑negative   */
    }
    last = n;
    return buf[n];
}
