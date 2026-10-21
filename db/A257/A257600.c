/*
 * A257600  Expansion of (4 + 15*x - 35*x^2 + 20*x^3 - 2*x^5)/(1 - x)^5.
 * https://oeis.org/A257600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 for n = 0; a(n) = (24 + 250*n + 125*n^2 + 20*n^3 +
 *            n^4)/12 for n > 0
 * Range:     u8 n <= 0, u16 n <= 11, u32 n <= 326, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;

    seq_t sn  = (seq_t)n;
    seq_t n2  = sn * sn;          /* n^2 */
    seq_t n3  = n2 * sn;          /* n^3 */
    seq_t n4  = n3 * sn;          /* n^4 */

    seq_t num =  (seq_t)24
               + (seq_t)250 * sn
               + (seq_t)125 * n2
               + (seq_t)20  * n3
               + n4;

    return num / 12;
}
