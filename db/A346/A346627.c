/*
 * A346627  G.f. A(x) satisfies: A(x) = 1 / (1 + x) + x * A(x)^3.
 * https://oeis.org/A346627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1 * a(n) = (-1)^n + Σ_{i=0}^{n-1} Σ_{j=0}^{n-i-1} *
 *            a(i)*a(j)*a(n-i-j-1) for n≥1 (OEIS A346627)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms and for the pairwise convolution
     * conv[k] = Σ_{i=0}^{k} a(i)*a(k-i).  The arrays grow as needed.
     */
    static seq_t *cache = NULL;
    static seq_t *conv  = NULL;
    static idx_t  last  = 0;   /* index of last computed a[] (≥0) */
    static idx_t  cap   = 0;   /* current allocation size */

    /* ---- allocate space for indices up to n --------------------------------- */
    if (cap <= n) {
        idx_t new_cap = (cap > 0 ? cap : 1);
        while (new_cap <= n) new_cap <<= 1;            /* double until big enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        conv  = (seq_t *)realloc(conv,  new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* ---- initialise a(0) and conv(0) -------------------------------------- */
    if (last == 0) {                     /* first or after reset               */
        cache[0] = (seq_t)1;             /* a(0) = 1                            */
        conv[0]  = (seq_t)1;             /* conv[0] = a(0)*a(0) = 1              */
    }

    /* ---- compute terms up to n --------------------------------------------- */
    while (last < n) {
        idx_t i = last + 1;              /* compute a[i]                        */

        /*  sum_{j=0}^{i-1} a(j) * conv[i-1-j]  */
        seq_t sum = 0;
        for (idx_t j = 0; j < i; ++j)
            sum += cache[j] * conv[i - 1 - j];

        /* a[i] = (-1)^i + sum  (with i odd → subtract 1) */
        if (i & 1u)
            cache[i] = sum - (seq_t)1;    /* odd i */
        else
            cache[i] = sum + (seq_t)1;    /* even i */

        /* update conv[i] = Σ_{j=0}^{i} a[j]*a[i-j] */
        seq_t csum = 0;
        for (idx_t j = 0; j <= i; ++j)
            csum += cache[j] * cache[i - j];
        conv[i] = csum;

        last = i;                       /* new last computed index           */
    }

    return cache[n];
}
