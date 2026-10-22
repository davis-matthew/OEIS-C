/*
 * A373397  Expansion of Sum_{k>=0} x^(6^k) / (1 - x^(6^k))^2.
 * https://oeis.org/A373397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n, if n ≢ 0 mod 6; a(n) = n + a(n/6) otherwise
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* to be safe for out‑of‑range calls */
    seq_t sum = (seq_t)n;
    while (n % 6 == 0)
    {
        n /= 6;
        sum += (seq_t)n;
    }
    return sum;
}
