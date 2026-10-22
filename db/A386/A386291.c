/*
 * A386291  Long leg of the unique primitive Pythagorean triple whose inradius
 *          is A000108(n) and such that its long leg and its hypotenuse are
 *          consecutive natural numbers.
 * https://oeis.org/A386291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * A000108(n) * (A000108(n) + 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t catalan = 1;                /* C_0 = 1 */
    for (idx_t i = 0; i < n; ++i) {
        /* C_{i+1} = C_i * 2*(2*i+1) / (i+2)  */
        seq_t numerator = catalan * ( (seq_t)(2 * (2*i + 1)) );
        catalan = numerator / (i + 2);
    }
    seq_t result = 2 * catalan * (catalan + 1);
    return result;
}
