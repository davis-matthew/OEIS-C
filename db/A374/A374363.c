/*
 * A374363  a(n) is the greatest term t <= n of A005836 such that n - t also
 *          belongs to A005836.
 * https://oeis.org/A374363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = take ternary digits of n, replace each 2 by 1. This
 *            yields the greatest term t ≤ n in A005836 such that n‑t also
 *            lies in A005836
 * Range:     u8 n <= 255, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power   = 1;
    while (n > 0)
    {
        idx_t digit = n % 3;
        if (digit == 2)
            digit = 1;
        result += (seq_t)digit * power;
        power   *= 3;
        n       /= 3;
    }
    return result;
}
