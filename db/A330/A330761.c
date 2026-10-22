/*
 * A330761  Array read by antidiagonals: T(n,k) is the number of faces on a
 *          ring formed by connecting the ends of a prismatic rod whose
 *          cross-section is an n-sided regular polygon after applying a twist
 *          of k/n turns.
 * https://oeis.org/A330761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = gcd(n,k) where the pairs (n,k) are read in antidiagonal *
 *            order from the array T(n,k)=gcd(n,k). The sequence starts with
 *            n=2, k=0
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t idx) {
    /* Guard against idx==0, though the sequence is defined for n>=1. */
    if (idx == 0) return (seq_t)0;

    idx_t cumulative = 0;   /* number of terms before the current diagonal */
    idx_t S = 2;            /* current diagonal sum (n + k) */

    /* Find the diagonal containing the idx‑th term. */
    while (cumulative + (S - 1) < idx) {
        cumulative += S - 1;  /* add the size of the previous diagonal */
        ++S;
    }

    /* Position within the diagonal (0‑based).  The diagonal is traversed
     * by decreasing n, so the first element corresponds to offset 0. */
    idx_t offset = idx - cumulative - 1;

    idx_t n_coord = S - offset;  /* n in the pair */
    idx_t k_coord = offset;      /* k in the pair */

    return gcd((seq_t)n_coord, (seq_t)k_coord);
}
