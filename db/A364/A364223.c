/*
 * A364223  Expansion of Sum_{k>=0} 5^k * x^(5^k) / (1 - x^(5^k))^2.
 * https://oeis.org/A364223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * (v5(n) + 1) where v5(n) is the exponent of 5 in n
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count how many times 5 divides n */
    idx_t tmp  = n;
    idx_t exp  = 0;
    while (tmp % 5 == 0) {
        exp++;
        tmp /= 5;
    }

    /* a(n) = n * (exp + 1) */
    return (seq_t)n * ((seq_t)exp + 1);
}
