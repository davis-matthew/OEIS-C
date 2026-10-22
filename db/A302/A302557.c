/*
 * A302557  Expansion of 1/(2 - Sum_{k>=0} k!*x^k/(1 + x)^(k+1)).
 * https://oeis.org/A302557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; For n≥1: D(0)=1, D(1)=0, D(k) = (k-1)*(D(k-1)+D(k-2))
 *            (the k‑th derangement) a(n) = Σ_{k=1..n} D(k) * a(n-k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses two dynamic arrays: one for the derangement
   numbers D(k) and one for the sequence a(k).  All arithmetic is done
   with seq_t operators, which work for 8/16/32/64‑bit integers, __int128,
   and arbitrary‑precision types (the latter provided by the build). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate space for derangements and a-sequence */
    seq_t *der   = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *aseq  = (seq_t*)malloc((n + 1) * sizeof(seq_t));

    if (!der || !aseq)        /* allocation failure – return 0 */
        return (seq_t)0;

    /* Base values for derangements */
    der[0] = (seq_t)1;
    if (n >= 1)
        der[1] = (seq_t)0;

    /* Pre‑compute derangements up to n */
    for (idx_t k = 2; k <= n; ++k) {
        /* (k-1) fits in idx_t → cast safely to seq_t */
        der[k] = (seq_t)(k - 1) * (der[k - 1] + der[k - 2]);
    }

    /* Base value for the sequence */
    aseq[0] = (seq_t)1;

    /* Compute a(k) for all k from 1 to n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += der[k] * aseq[i - k];
        }
        aseq[i] = sum;
    }

    seq_t result = aseq[n];

    /* Clean up */
    free(der);
    free(aseq);

    return result;
}
