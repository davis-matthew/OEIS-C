/*
 * A277679  Start with 1,2,3,4,5,6,.... For n >=1, remove the first n terms
 *          and reverse the remaining terms n+1 at a time. Concatenate the
 *          terms removed. (See the example.)
 * https://oeis.org/A277679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th term removed when repeatedly removing the first
 *            k terms and reversing the rest in blocks of length k+1. It is
 *            computed by backtracking the position of the removed element
 *            through the successive block reversals
 * Range:     u8 n <= 190, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   triangular_step
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation works for any idx_t/seq_t types provided by the
   build.  It uses only integer arithmetic, making it valid both for
   C11 and C++17. */

#include <stdint.h>
#include "../utils/triangular_step.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* sequence is 1‑based; return 0 for 0 */
        return 0;

    /* Determine which step (k) contains the n‑th removal. */
    idx_t k = triangular_step(n);            /* minimal k with k*(k+1)/2 >= n */

    /* Previous triangular number: T_{k-1} = (k-1)*k/2 */
    idx_t prev = k - 1;
    __uint128_t t_prev128 = (__uint128_t)prev * (prev + 1ULL) / 2ULL;
    idx_t T_prev = (idx_t)t_prev128;

    /* Position inside the current step (0‑based). */
    idx_t offset = n - T_prev;               /* 1 … k */
    seq_t pos = (seq_t)(offset - 1);         /* 0‑based index in L_{k-1} */

    /* Back‑track through the k‑1 earlier steps to find the original
       index in the identity permutation. */
    for (idx_t step = k - 1; step >= 1; --step) {
        seq_t B = (seq_t)(step + 1);        /* block size at this step (k+1) */
        seq_t q = pos / B;
        seq_t r = pos % B;
        seq_t rev = (q + 1) * B - 1 - r;   /* reverse position within the block */
        pos = (seq_t)step + rev;           /* map to previous list */
    }

    /* The removed value is original index + 1. */
    return pos + 1;
}
