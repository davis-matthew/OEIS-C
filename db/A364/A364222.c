/*
 * A364222  Expansion of Sum_{k>=0} 3^k * x^(3^k) / (1 - x^(3^k))^2.
 * https://oeis.org/A364222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n + 3*a(n/3) if n ≡ 0 (mod 3); otherwise a(n) = n
 * Range:     u8 n <= 80, u16 n <= 8747, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Recursive definition from the OEIS entry.) */
seq_t a(idx_t n)
{
    if (n == 0)        /* safety guard – not used for valid indices */
        return 0;
    if ((n % 3) == 0)
        return (seq_t)n + (seq_t)3 * a(n / 3);
    return (seq_t)n;
}
