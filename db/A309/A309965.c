/*
 * A309965  a(n) is the greatest product of the digits of n in any base b in
 *          the range 2..n.
 * https://oeis.org/A309965
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 287, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = max_{b=2..n} (product of the digits of n when written in base b) */

seq_t a(idx_t n)
{
    seq_t best = (seq_t)0;
    for (idx_t b = (idx_t)2; b <= n; ++b)
    {
        idx_t temp = n;
        seq_t prod = (seq_t)1;
        while (temp != 0)
        {
            seq_t digit = (seq_t)(temp % b);
            prod *= digit;
            if (prod == 0) break;
            temp /= b;
        }
        if (prod > best) best = prod;
    }
    return best;
}
