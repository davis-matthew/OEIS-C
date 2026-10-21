/*
 * A272212  Sum of the odd numbers among the larger parts of the partitions of
 *          n into two parts.
 * https://oeis.org/A272212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{i=1}^{⌊n/2⌋} (n-i)*((n-i) mod 2)
 * Range:     u8 n <= 37, u16 n <= 591, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: sum of all odd integers L with ⌈n/2⌉ ≤ L ≤ n-1 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* no partitions for n = 0 or 1 */

    /* Start of the interval: ceil(n/2) */
    idx_t start_idx = n - (n/2);   /* ceil(n/2) using integer division */
    seq_t start = (seq_t)start_idx;
    seq_t end   = (seq_t)(n - 1);

    /* First and last odd numbers in the interval */
    seq_t start_odd = (start & 1) ? start : start + 1;
    seq_t end_odd   = (end   & 1) ? end   : end   - 1;

    if (start_odd > end_odd)
        return (seq_t)0;          /* empty interval */

    /* Count of odd terms: (last - first)/2 + 1 */
    seq_t count = ((end_odd - start_odd) / 2) + 1;

    /* Sum of arithmetic progression: count * (first + last) / 2 */
    seq_t sum = count * ((start_odd + end_odd) >> 1);

    return sum;
}
