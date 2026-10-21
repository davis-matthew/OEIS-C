/*
 * A254231  Product of tribonacci numbers A000073(2) * ... * A000073(n).
 * https://oeis.org/A254231
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=2}^n T(k), where T(k) is the tribonacci sequence
 *            A000073
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    seq_t prod = (seq_t)1;
    seq_t t0   = (seq_t)0;
    seq_t t1   = (seq_t)0;
    seq_t t2   = (seq_t)1;

    /* include T(2) in the product */
    prod *= t2;

    for (idx_t k = 3; k <= n; ++k)
    {
        seq_t t3 = t2 + t1 + t0;
        prod *= t3;
        t0 = t1;
        t1 = t2;
        t2 = t3;
    }

    return prod;
}
