/*
 * A333557  a(n) = Sum_{d|n, gcd(d, n/d) = 1} uphi(d) * uphi(n/d), where uphi
 *          = unitary totient function (A047994).
 * https://oeis.org/A333557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^k||n} 2*(p^k-1)
 * Range:     u8 n <= 83, u16 n <= 7139, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n >= 1; a(1)=1 automatically from empty product. */
    seq_t result = 1;
    idx_t temp   = n;

    /* Trial division up to sqrt(temp). */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            seq_t pk = 1;          /* = p^k */
            while (temp % p == 0) {
                temp /= p;
                pk *= (seq_t)p;
            }
            result *= (seq_t)2 * (pk - 1);
        }
    }

    /* If a prime factor remains. */
    if (temp > 1) {
        seq_t pk = (seq_t)temp;    /* p^1 */
        result *= (seq_t)2 * (pk - 1);
    }

    return result;
}
