/*
 * A328371  Irregular triangle read by rows: T(n,k) is the sum of all parts of
 *          all partitions of all positive integers <= n into k consecutive
 *          parts.
 * https://oeis.org/A328371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A328371.  The triangle value T(r,k)
   (sum of all parts in all partitions of all integers ≤ r into k
   consecutive parts) has the closed form

          T(r,k) = k*A*(A+1)/2 + A*k(k-1)/2,
   where A = ⌊(r – k(k-1)/2)/k⌋.

   The sequence a(n) is obtained by reading this triangle row‑by‑row:
   a(n) = T(r,k) where r is the row containing the n‑th entry and
   k is its 1‑based column within that row. */

#include <stdint.h>
#include <stdlib.h>

/* Compute a(n) for n ≥ 1.  The function conforms to the build
   supplied types seq_t (value) and idx_t (index). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* out of bounds – return 0 for safety */

    idx_t row = 0;
    idx_t cumulative = 0;          /* number of entries up to but not
                                      including current row */

    /* Find the row that contains index n. */
    for (;;)
    {
        ++row;

        /* Determine kmax: largest k with k(k+1)/2 ≤ row. */
        idx_t k = 1;
        while (((k * (k + 1)) / 2) <= row)
            ++k;
        idx_t kmax = k - 1;

        if (cumulative + kmax >= n)
            break;
        cumulative += kmax;
    }

    /* Position in the row: 0‑based offset. */
    idx_t offset = n - cumulative - 1;
    idx_t k = offset + 1;          /* 1‑based column index */

    /* Convert to seq_t for arithmetic. */
    seq_t kseq  = (seq_t)k;
    seq_t rseq  = (seq_t)row;

    /* Compute A = floor((r – k(k-1)/2)/k). */
    seq_t numerator   = rseq - (kseq * (kseq - (seq_t)1)) / (seq_t)2;
    seq_t Amax        = numerator / kseq;

    /* Compute T(r,k) using the closed form. */
    seq_t term1 = kseq * Amax * (Amax + (seq_t)1) / (seq_t)2;
    seq_t term2 = (kseq * (kseq - (seq_t)1)) / (seq_t)2 * Amax;
    return term1 + term2;
}
