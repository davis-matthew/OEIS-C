/*
 * A352269  Irregular triangle read by rows: T(n,k) is the number of parts in
 *          the partition of n into 2*k-1 consecutive parts, 1 <= k <=
 *          A351846(n). T(n,k) = 0 if no such partition exists.
 * https://oeis.org/A352269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 2246, u16 n <= 9896, u32 n <= 9896, u64 n <= 9896,
 *            u128 n <= 9896, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352269  
   Read the irregular triangle T(r,k) row‑wise.  
   For each row r (r ≥ 1) let kmax be the largest k with k·(2k–1) ≤ r.  
   The row consists of kmax terms:
       T(r,k) = (2k–1)   if (2k–1) divides r
                 0       otherwise
   The sequence is the flattened triangle: a(1), a(2), …, a(n) are the
   successive terms produced by traversing rows r = 1,2,… and within a
   row the terms for k = 1..kmax.  */

/* Include stdbool.h for bool type (not used here but required by build). */
#include <stdbool.h>

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for offset < 1, but safeguard */
        return 0;

    idx_t cumulative = 0;   /* number of elements produced so far   */
    idx_t r = 1;            /* current row number                   */

    while (1) {
        /* Find largest k such that k*(2k-1) ≤ r   (k ≥ 1)   */
        idx_t kmax = 0;
        for (idx_t k = 1; ; ++k) {
            seq_t prod = (seq_t)k * (2 * (seq_t)k - 1);
            if (prod > (seq_t)r) {
                kmax = k - 1;
                break;
            }
        }

        if (cumulative + kmax >= n) {
            /* The desired position is within this row. */
            idx_t offset = n - cumulative;          /* 1‑based offset in the row */
            seq_t d = 2 * (seq_t)offset - 1;        /* candidate divisor      */
            if (r % (idx_t)d == 0)                  /* does d divide r?      */
                return d;
            else
                return 0;
        }

        cumulative += kmax;   /* skip over this entire row          */
        ++r;
    }
}
