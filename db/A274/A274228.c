/*
 * A274228  Triangle read by rows: T(n,k) (n>=3, 0<=k<=n-3) = number of
 *          n-sequences of 0's and 1's with exactly one pair of adjacent 0's
 *          and exactly k pairs of adjacent 1's.
 * https://oeis.org/A274228
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   binom, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes sequence A274228.
   The sequence is obtained by reading the triangle
   T(n,k) (n ≥ 3, 0 ≤ k ≤ n–3) left‑to‑right, row by row,
   where T(n,k) is the number of binary strings of length n
   with exactly one pair of adjacent 0’s and exactly k pairs
   of adjacent 1’s.  
   The triangular entry is given by

        T(n,k) = (k+1)*( binom( ⌊(n+k-2)/2⌋ , k+1 )
                      + binom( ⌊(n+k-3)/2⌋ , k+1 ))
                + 2*binom( ⌊(n+k-3)/2⌋ , k ).

   The function a(n) is the element of this triangle
   that lies at position n in the flattened sequence
   (with offset 3, so a(3) is the first element).
*/

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* The sequence starts at offset 3.  For n < 3 the value is undefined
       (returning 0). */
    if (n < 3) return 0;

    /* Find the row r such that
          start(r) = 3 + (r-3)(r-2)/2  <= n < start(r+1)            */
    idx_t m = n - 3;                            /* m ≥ 0             */

    /* Solve (x)(x+1) ≤ 2*m where x = r-3.
       D = 1 + 8*m fits in 64‑bit for the problem limits.        */
    idx_t D = 1 + (idx_t)8 * m;
    seq_t D_seq = (seq_t)D;
    seq_t sqrt_D_seq = floor_sqrt(D_seq);
    idx_t sqrt_D = (idx_t)sqrt_D_seq;

    idx_t x = (sqrt_D - 1) / 2;                 /* r = x + 3           */
    idx_t r = x + 3;

    /* Compute start(r).  Using 64‑bit arithmetic – no overflow for n≤10000. */
    idx_t start = 3 + (r - 3) * (r - 2) / 2;
    /* Adjustment – because floor may give the previous row. */
    if (n >= start + (r - 2)) {
        r++;
        start = 3 + (r - 3) * (r - 2) / 2;
    }

    idx_t k = n - start;                       /* 0 ≤ k ≤ r-3          */

    /* T(n,k) = (k+1)*(binom(A,k+1)+binom(B,k+1)) + 2*binom(B,k)  */
    idx_t A = (r + k - 2) >> 1;                /* ⌊(n+k-2)/2⌋          */
    idx_t B = (r + k - 3) >> 1;                /* ⌊(n+k-3)/2⌋          */

    seq_t term1 = binom(A, k + 1);
    seq_t term2 = binom(B, k + 1);
    seq_t term3 = binom(B, k);

    seq_t result = ((seq_t)(k + 1)) * (term1 + term2) + ((seq_t)2) * term3;
    return result;
}
