/*
 * A272104  Sum of the even numbers among the larger parts of the partitions
 *          of n into two parts.
 * https://oeis.org/A272104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of even integers i with ceil(n/2) ≤ i ≤ n‑1.
 *            Equivalently a(n) = Σ_{i=1}^{⌊n/2⌋} (n‑i)·((n‑i+1) mod 2)
 * Range:     u8 n <= 38, u16 n <= 590, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 gives a(0) = 0 by definition.  */
    if (n <= 1)
        return 0;

    /* start = ceil(n/2)  – use integer arithmetic.  */
    idx_t start = (n + 1) / 2;
    /* end = n‑1, at least 0 because n ≥ 2 here.  */
    idx_t end   = n - 1;

    /* First even number ≥ start.  */
    idx_t e_start = (start & 1) ? start + 1 : start;
    /* Last even number ≤ end.  */
    idx_t e_end   = (end & 1) ? end - 1 : end;

    if (e_start > e_end)
        return 0;

    /* Number of even terms in the range.  */
    idx_t t = ((e_end - e_start) / 2) + 1;

    /* Arithmetic mean of the first and last even terms.  */
    seq_t avg = ((seq_t)e_start + (seq_t)e_end) / 2;

    /* Total sum = number of terms × average.  */
    return (seq_t)t * avg;
}
