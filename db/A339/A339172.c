/*
 * A339172  Number of compositions (ordered partitions) of n into distinct
 *          parts, the least being 9.
 * https://oeis.org/A339172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A339172: Number of compositions (ordered partitions) of n into distinct parts, the least being 9.
   Each composition corresponds to a subset S of distinct integers ≥9 that sums to n,
   followed by an ordering of S.  Thus
        a(n) = Σ_{S⊆{9,10,…}, sum(S)=n} |S|!.
   The algorithm counts, for every k, the number of such subsets of size k
   using a standard partition dynamic program, then multiplies by k! and sums.  */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n < 9 there are no admissible compositions. */
    if (n < 9)
        return 0;

    /* Determine maximal number of parts kmax that can fit into n
       starting with the smallest part 9.  */
    idx_t kmax = 0;
    idx_t sum  = 0;
    for (;; ++kmax) {
        idx_t next = 9 + kmax;           /* next distinct part if we use kmax+1 parts */
        if (sum + next > n)
            break;
        sum += next;
    }

    /* Allocate a working array of size n+1 for the inner partition DP
       (the type seq_t may be an arbitrary‑precision integer).  */
    seq_t *c = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!c)
        return 0;                      /* allocation failure – unlikely for typical inputs */

    seq_t result = 0;
    seq_t fact   = 1;                  /* fact = k!   (fact(0) = 1) */

    /* For each k from 1 to kmax compute the number of subsets of size k.
       shift = minimal sum of k distinct parts (9 + 10 + … + 9+k-1).  */
    for (idx_t k = 1; k <= kmax; ++k) {
        /* Shift of the generating function for size k */
        idx_t shift = k * 9 + k * (k - 1) / 2;

        /* If even the minimal sum exceeds n, further k's will also be too large. */
        if (shift > n)
            break;

        /* Reset coefficient array to zero, except c[0] = 1. */
        for (size_t i = 0; i <= (size_t)n; ++i)
            c[i] = 0;
        c[0] = 1;

        /* Convolution with 1/(1 - x^j) for j = 1 … k-1:
           this builds the number of partitions into parts of sizes 1..k-1
           (each size may appear any number of times).  */
        for (idx_t j = 1; j <= k - 1; ++j) {   /* no parts for k = 1, loop skipped */
            for (idx_t t = j; t <= n; ++t) {
                c[t] += c[t - j];
            }
        }

        /* After the convolution, c[t] holds the number of subsets of size k
           that sum to (t + shift).  Thus we need the coefficient at n - shift. */
        idx_t index = n - shift;
        result += c[index] * fact;

        /* Prepare factorial for next k: fact becomes (k+1)! */
        fact *= (seq_t)(k + 1);
    }

    free(c);
    return result;
}
