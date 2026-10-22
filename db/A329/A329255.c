/*
 * A329255  Irregular triangle read by rows: T(n,k) is greatest positive
 *          integer <= n that have a partition into k consecutive parts, 1 <=
 *          k <= A003056(n), n >= 1.
 * https://oeis.org/A329255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A329255 – Irregular triangle read by rows
 *
 *  a(n) is the nth element of the irregular triangle T(n,k) where
 *  T(n,k) = the greatest integer m ≤ n that can be written as a sum of
 *  k consecutive positive integers.  For each n, the row indices k
 *  run from 1 to A003056(n) = ⌊(√(8n+1)–1)/2⌋, the largest k for which
 *  the sum of the first k natural numbers does not exceed n.
 *
 *  The triangle is read rowwise: (1); (2); (3,3); (4,3); (5,5); ...
 *  This function returns the a(n) term for any n ≥ 1.
 *
 *  Implementation details
 *
 *  - Find the row r such that the cumulative number of triangle entries
 *    up to row r‑1 is < n ≤ cumulative up to row r.  The row length
 *    satisfies len(r) = ⌊(√(8r+1) – 1)/2⌋.
 *
 *  - For row r the column index is k = n – cumulative_prev.
 *
 *  - T(r,k) can be computed directly:
 *        let T_k = k(k+1)/2   (triangular number of k)
 *        then T(r,k) = r – ((r – T_k) mod k).
 *
 *  All arithmetic is performed in the build‑defined type seq_t.
 *  Only integer arithmetic and standard headers are used.  The routine
 *  runs in O(√n) time (for n ≤ 10 000 r ≤ 150) and uses O(1) space.
 */

#include <stdint.h>   /* for uint64_t and other uint* types */

/* Return the a(n) term for the irregular triangle described above */
seq_t a(idx_t n)
{
    /* Find the row that contains the nth entry. */
    idx_t cumulative = 0;     /* number of entries before the current row   */
    idx_t r = 0;              /* current row (1‑based)                        */
    idx_t len = 0;            /* length of the current row                    */

    /* Incrementally maintain len: the largest integer m such that
       m(m+1)/2 ≤ r.  The condition can be checked with a 128‑bit
       product to avoid overflow. */
    while (1)
    {
        ++r;   /* move to next row */

        /* Update len for the new r.  The loop increases len by at most 1
           per iteration, keeping the algorithm linear in r. */
        while (1)
        {
            __uint128_t next = (__uint128_t)(len + 1) * (len + 2) / 2;
            if (next <= r)
                ++len;
            else
                break;
        }

        /* If the n-th term lies in this row, stop; otherwise
           accumulate the row length and continue. */
        if (cumulative + len >= n)
            break;
        cumulative += len;
    }

    /* Column index within the found row (1‑based). */
    idx_t k = n - cumulative;

    /* Compute T(r,k) = r – ((r – T_k) mod k) where T_k = k(k+1)/2. */
    seq_t T_k   = ((seq_t)k * ((seq_t)k + 1)) / 2;
    seq_t rem   = (((seq_t)r - T_k) % (seq_t)k);
    return ((seq_t)r - rem);
}
