/*
 * A370180  Size of the group Z_3*_/(Z_3*)^n, where Z_3 is the ring of 3-adic
 *          integers.
 * https://oeis.org/A370180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 3^v₃(n) * gcd(2, n / 3^v₃(n))
 * Range:     u8 n <= 485, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t temp = n;
    idx_t e3 = 0;
    while ((temp % 3) == 0) {
        temp /= 3;
        ++e3;
    }
    /* temp now equals n / 3^e3, not divisible by 3 */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < e3; ++i) {
        pow3 *= (seq_t)3;
    }
    seq_t result = pow3;
    /* gcd(2, temp) is 2 if temp is even, else 1 */
    if ((temp & 1) == 0) {
        result *= (seq_t)2;
    }
    return result;
}
