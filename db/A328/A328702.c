/*
 * A328702  Start with 0, a(n) is the smallest number of iterations: x ->
 *          (x^2+x+1) mod n needed to run into a cycle.
 * https://oeis.org/A328702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A328702 :
   a(n) = minimal i such that f(i)=f(j) for some j>i,
   where f(0)=0 and f(k+1)=(f(k)^2+f(k)+1) mod n.
   Implementation finds the first repeated residue by storing the
   first‑occurrence index of each value modulo n.  Complexity O(n). */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n <= 1 – the sequence is defined for n >= 1. */
    if (n <= 1)
        return (seq_t)0;

    /* Allocate an array of indices for first occurrences of residues. */
    size_t sz = (size_t)n;
    idx_t *first = (idx_t*)malloc(sz * sizeof(idx_t));
    if (!first)
        return (seq_t)0;                 /* allocation failure */

    for (idx_t i = 0; i < n; ++i)
        first[i] = (idx_t)-1;            /* sentinel = not seen */
    first[0] = 0;                         /* f(0)=0 at index 0 */

    idx_t   step = 0;                     /* current index k */
    seq_t   f    = (seq_t)0;              /* current value f(k) */

    for (;;)
    {
        ++step;                          /* compute f(k+1) */
        f = (f * f + f + 1) % (seq_t)n;  /* f(k+1) modulo n */

        idx_t residue = (idx_t)f;        /* 0 <= residue < n */

        if (first[residue] == (idx_t)-1)
            first[residue] = step;       /* first time this residue appears */
        else
        {
            idx_t ans = first[residue];  /* earliest repetition index */
            free(first);
            return (seq_t)ans;
        }
    }
}
