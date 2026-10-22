/*
 * A350166  Partial sums of A050469.
 * https://oeis.org/A350166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} k if floor(n/k) mod 4 ∈ {1,2}, otherwise 0
 * Range:     u8 n <= 23, u16 n <= 377, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = n / k;
        unsigned r = (unsigned)(t & 3);           /* r = t mod 4 */
        if (r == 1 || r == 2)
            sum += (seq_t)k;
    }
    return sum;
}
