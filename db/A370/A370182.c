/*
 * A370182  Size of the group Z_7*_/(Z_7*)^n, where Z_7 is the ring of 7-adic
 *          integers.
 * https://oeis.org/A370182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = (n%2==0?2:1) * (n%3==0?3:1) * 7^{v_7(n)}
 * Range:     u8 n <= 293, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  v_7(n) = the exponent of 7 in n.  For primes other than 2,3,7 the
 *  contribution is 1, so they can be ignored. */
seq_t a(idx_t n)
{
    seq_t result = 1;

    /* factor 2 */
    if ((n & 1) == 0)          /* n % 2 == 0 */
        result *= (seq_t)2;

    /* factor 3 */
    if (n % 3 == 0)
        result *= (seq_t)3;

    /* factor 7^e */
    idx_t t = n;
    seq_t pow7 = 1;
    while (t % 7 == 0) {
        pow7 *= (seq_t)7;
        t /= 7;
    }
    result *= pow7;

    return result;
}
