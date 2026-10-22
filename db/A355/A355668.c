/*
 * A355668  Array read by upwards antidiagonals T(n,k) = J(k) + n*J(k+1) where
 *          J(n) = A001045(n) is the Jacobsthal numbers.
 * https://oeis.org/A355668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = J(k) + (d - k) * J(k+1) where d = floor((sqrt(1+8n)-1)/2)
 *            is the diagonal index, offset = n - d*(d+1)/2, k = offset, J(m)
 *            = Jacobsthal number: J(0)=0, J(1)=1, J(m+2) = J(m+1) + 2*J(m)
 * Range:     u8 n <= 52, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following C/C++ function computes OEIS sequence A355668,
   defined as the array of T(n,k) = J(k)+n*J(k+1) read along
   upwards antidiagonals.  It uses only integer arithmetic,
   memoises Jacobsthal numbers up to the required index, and
   works for any n up to at least 10000. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        d++;

    /* Offset within the diagonal and back‑substitution for (i,k) */
    idx_t offset = n - d * (d + 1) / 2;
    idx_t i = d - offset;
    idx_t k = offset;

    /* Memoise Jacobsthal numbers J[0...k+1] */
    static seq_t J[200];            /* sufficient for n <= 10000 (k <= 141) */
    static size_t J_last = 0;       /* highest index currently computed */
    static int J_inited = 0;        /* flag to initialise J[0] and J[1] */

    if (!J_inited) {
        J[0] = 0;
        J[1] = 1;
        J_last = 1;
        J_inited = 1;
    }

    size_t need = (size_t)k + 1;    /* we need J[k] and J[k+1] */
    if (need > J_last) {
        for (size_t idx = J_last + 1; idx <= need; ++idx) {
            seq_t twice = J[idx - 2] + J[idx - 2];
            J[idx] = J[idx - 1] + twice;     /* J[idx] = J[idx-1] + 2*J[idx-2] */
        }
        J_last = need;
    }

    /* Compute the term: J(k) + i * J(k+1) */
    seq_t result = J[k] + (seq_t)i * J[k + 1];
    return result;
}
