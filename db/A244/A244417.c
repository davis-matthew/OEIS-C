/*
 * A244417  Exponents of 6 in appearing in the 6-adic value of 1/n, n>=1
 *          (A244416).
 * https://oeis.org/A244417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max(v2(n), v3(n)) where v2(n) is the exponent of 2 in n's
 *            prime factorization and v3(n) is the exponent of 3
 * Range:     u8 n <= 9089, u16 n <= 9089, u32 n <= 9089, u64 n <= 9089,
 *            u128 n <= 9089, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    seq_t v2 = 0;
    while ((tmp & 1ULL) == 0ULL) {
        v2++;
        tmp >>= 1;
    }

    tmp = n;
    seq_t v3 = 0;
    while (tmp % 3ULL == 0ULL) {
        v3++;
        tmp /= 3;
    }

    return (v2 > v3) ? v2 : v3;
}
