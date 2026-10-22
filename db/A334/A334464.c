/*
 * A334464  a(n) is the total number of parts in all partitions of n into
 *          consecutive parts that differ by 4.
 * https://oeis.org/A334464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1} k if (n + 2k(k‑1)) is divisible by k and (n +
 *            2k(k‑1))/k ≥ 4k‑3 where k is the number of parts. This counts
 *            all partitions of n into consecutive parts that differ by 4,
 *            summing the number of parts over all such partitions
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; ; ++k) {
        /* minimal sum achievable with k parts:
           2*k*k - k ≤ n must hold for any solution. */
        seq_t min_sum = (seq_t)2 * k * k - k;
        if (min_sum > n) break;

        seq_t num = (seq_t)n + (seq_t)2 * k * (k - 1);
        if (num % k != 0) continue;          /* a must be integer */

        seq_t a_val = num / k;                /* first part of the progression */
        if (a_val >= 4 * (seq_t)k - 3)        /* smallest part ≥ 1 */
            total += (seq_t)k;
    }
    return total;
}
