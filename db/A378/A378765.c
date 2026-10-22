/*
 * A378765  (Exponent of the highest power of 2 that divides n!), reduced
 *          modulo 10.
 * https://oeis.org/A378765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A011371(n) mod 10
 * Range:     u8 n <= 312, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A011371(n) is the exponent of the highest power of 2 that divides n! */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t div = (seq_t)n;
    while (div)
    {
        div >>= 1;          /* div = floor(div / 2) */
        sum += div;         /* accumulate floor(n / 2^k) for k >= 1 */
    }
    return sum % 10;        /* reduce modulo 10 */
}
