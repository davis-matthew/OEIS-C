/*
 * A296768  Initially let b(i)=i for all i > 0. On the k-th pass, exchange
 *          b(k+1) with b(b(k+1) + b(k)). The sequence is the limit of these
 *          sequences as k goes to infinity.
 * https://oeis.org/A296768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   ensure_b
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296768 – absolutely convergent lazy permutation
   Definition: b(i)=i for i>0.  
   On pass k (k≥1) swap b(k+1) with b(b(k+1)+b(k)).
   a(n) is the value of b(n) after the permutation has stabilized
   (i.e., after the first n−1 passes).  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ensure_b.h"

/* Return the n-th term of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* safety for non‑positive indices */

    static seq_t *perm = NULL;             /* permutation array */
    static idx_t cap   = 0;                /* current allocation size   */
    static idx_t last  = 0;                /* last processed k (k+1 swapped) */

    /* make sure we have storage up to index n */
    if (cap <= n) ensure_b(&perm, &cap, n);

    /* perform missing passes (k from last+1 to n-1) */
    if (n > last + 1) {
        for (idx_t k = last + 1; k < n; ++k) {
            idx_t i  = k + 1;               /* left index in this pass */
            seq_t val_left = perm[i];       /* current value at i   */
            seq_t val_k    = perm[k];       /* current value at k   */
            idx_t j = (idx_t)(val_left + val_k); /* right index */
            ensure_b(&perm, &cap, j);
            /* swap b[i] with b[j] */
            seq_t tmp = perm[i];
            perm[i] = perm[j];
            perm[j] = tmp;
        }
        last = n - 1;                      /* all k < n now processed */
    }

    return perm[n];
}
