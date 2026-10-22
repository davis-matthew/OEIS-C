/*
 * A363526  Number of integer partitions of n with reverse-weighted sum 3*n.
 * https://oeis.org/A363526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the integer partitions of n whose reverse-weighted
 *            sum equals 3*n. From the analysis it follows that for n > 0 only
 *            partitions with 4 or 5 parts can satisfy the condition. k = 5:
 *            (n/5) repeated parts if n divisible by 5 → exactly one
 *            partition. k = 4: Let the differences between conse...
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    idx_t N = n;
    idx_t count = 0;

    /* k = 5 : only if N divisible by 5 */
    if ((N % 5) == 0) count++;

    /* k = 4 : enumerate possible d2 */
    for (idx_t d2 = 0; d2 <= N / 4; ++d2) {
        idx_t rem = N - 4 * d2;
        if (rem % 3 != 0) continue;
        idx_t s = rem / 3;                 /* s = d1 + d3 */

        /* smallest acceptable d1: max(0, 2 - d2) , with parity same as d2 */
        idx_t min_d1 = 2 > d2 ? (2 - d2) : 0;
        if (min_d1 > s) continue;
        if ((min_d1 & 1) != (d2 & 1)) {  /* adjust to required parity */
            ++min_d1;
        }
        if (min_d1 > s) continue;

        /* number of d1 in [min_d1, s] with step 2 */
        idx_t terms = (s - min_d1) / 2 + 1;
        count += terms;
    }

    return (seq_t)count;
}
