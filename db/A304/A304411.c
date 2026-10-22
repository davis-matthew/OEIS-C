/*
 * A304411  If n = Product (p_j^k_j) then a(n) = Product ((p_j + 1)*k_j).
 * https://oeis.org/A304411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^k||n} (p + 1)*k
 * Range:     u8 n <= 131, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    seq_t result = 1;
    idx_t m = n;
    /* factor for prime 2 */
    if (m % 2 == 0) {
        idx_t k = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++k;
        }
        result *= ((seq_t)2 + 1) * (seq_t)k;
    }
    /* factors for odd primes */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t k = 0;
            while (m % p == 0) {
                m /= p;
                ++k;
            }
            result *= ((seq_t)p + 1) * (seq_t)k;
        }
    }
    /* remaining prime factor, if any */
    if (m > 1) {
        result *= ((seq_t)m + 1) * 1;
    }
    return result;
}
