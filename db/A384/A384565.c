/*
 * A384565  Consecutive states of the linear congruential pseudo-random number
 *          generator 5*s mod 2^35 when started at s=1.
 * https://oeis.org/A384565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 * a(n-1) mod 2^35, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Modulus 2^35 */
    const seq_t MOD  = ((seq_t)1 << 35);
    const seq_t MASK = MOD - 1;

    if (n == 1) return (seq_t)1;

    idx_t exp = n - 1;            /* exponent for 5^(n-1) */

    seq_t result = 1;            /* current power of 5^k */
    seq_t base = 5;              /* 5^(2^i) at each step */

    while (exp) {
        if (exp & 1)
            result = (result * base) & MASK;
        base = (base * base) & MASK;
        exp >>= 1;
    }
    return result;
}
