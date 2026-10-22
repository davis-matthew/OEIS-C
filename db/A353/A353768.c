/*
 * A353768  a(n) = phi(n) mod 4; Euler totient function reduced modulo 4.
 * https://oeis.org/A353768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n) mod 4
 * Range:     u8 n <= 9534, u16 n <= 9534, u32 n <= 9534, u64 n <= 9534,
 *            u128 n <= 9534, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* φ(1) = 1, so start with that value */
    unsigned int mod = 1;

    idx_t m = n;

    /* factorize m and compute the multiplicative contribution
       of each prime power to φ(m) modulo 4. */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            unsigned int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* compute p^(e-1) modulo 4 */
            unsigned int powmod = 1;
            for (unsigned int i = 1; i < e; ++i) {
                powmod = (powmod * (p % 4)) % 4;
            }
            /* term = (p-1) * p^(e-1) modulo 4 */
            unsigned int term = (((p - 1) % 4) * powmod) % 4;
            mod = (mod * term) % 4;
        }
    }

    /* if what's left is a prime factor > sqrt(n) */
    if (m > 1) {
        /* contribution of that prime: (m-1) modulo 4 */
        unsigned int term = ((m % 4) + 3) % 4;   /* (m-1)%4 */
        mod = (mod * term) % 4;
    }

    return (seq_t)mod;
}
