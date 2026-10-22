/*
 * A372216  Product of n-th Pell number and n-th Catalan number: a(n) =
 *          A000129(n)*A000108(n).
 * https://oeis.org/A372216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Pell(n) * Catalan(n) with Pell(0)=0, Pell(1)=1,
 *            Pell(n)=2*Pell(n-1)+Pell(n-2) and
 *            Catalan(n)=binomial(2n,n)/(n+1) (supplied in utils)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/catalan.h"

static seq_t *Pell_cache = NULL;   /* cache of Pell numbers (index 0..). */
static idx_t  Pell_size  = 0;      /* allocated size (=max index+1). */

/* Compute Pell(n) with caching.  n>=1. */
static seq_t pell(idx_t n)
{
    if (n == 0)
        return 0;   /* not used for valid n, but harmless. */

    /* Grow cache if needed. */
    if (n >= Pell_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(Pell_cache, new_size * sizeof(seq_t));
        if (!tmp)     /* allocation failure – fall back to 0. */
            return 0;
        Pell_cache = tmp;

        /* Ensure smallest elements are initialised. */
        if (Pell_size == 0) {
            Pell_cache[0] = 0;   /* unused but convenient. */
            if (new_size > 1)
                Pell_cache[1] = 1;
        }
        /* If cache was empty, set size appropriately. */
        if (Pell_size < 2)
            Pell_size = 2;

        /* Fill the new entries. */
        for (idx_t i = Pell_size; i <= n; ++i) {
            /* For i==0,1 we have the initial values already. */
            if (i >= 2)
                Pell_cache[i] = 2 * Pell_cache[i-1] + Pell_cache[i-2];
        }
        Pell_size = new_size;
    }
    return Pell_cache[n];
}

/* Return a(n) for the sequence A372216.  n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* behaviour below offset not required. */
    seq_t p = pell(n);
    seq_t c = catalan(n);   /* provided by utils. */
    return p * c;
}
