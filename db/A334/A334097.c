/*
 * A334097  a(n) is the exponent of the eventual power of 2 reached when
 *          starting from n and using the map k -> k + k/p, where p can be any
 *          odd prime factor of k, for example, the largest.
 * https://oeis.org/A334097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the exponent of the eventual power of 2 reached from n
 *            using the map k -> k + k/p for an odd prime factor p of k. The
 *            sequence is totally additive: a(1) = 0, a(2) = 1, a(p) = 1 +
 *            a((p + 1) / 2) for odd primes p, a(m·n) = a(m) + a(n) for m, n >
 *            1 Thus a(n) can be computed by prime‑factoris...
 * Range:     u8 n <= 9858, u16 n <= 9858, u32 n <= 9858, u64 n <= 9858,
 *            u128 n <= 9858, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    seq_t result = 0;
    idx_t temp = n;

    /* Trial division to get prime factors */
    while (temp > 1) {
        /* Find smallest prime factor p of temp */
        idx_t p = 2;
        while (p * p <= temp && (temp % p) != 0) {
            ++p;
        }
        if (p * p > temp) { /* temp is prime itself */
            p = temp;
        }

        /* Count multiplicity of p */
        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        /* Compute a(p) */
        seq_t ap;
        if (p == 2) {
            ap = (seq_t)1;               /* a(2) = 1 */
        } else {
            /* a(p) = 1 + a((p + 1)/2) for odd primes */
            idx_t q = (p + 1) / 2;
            ap = (seq_t)1 + a(q);
        }

        /* Add contribution of this prime factor */
        result += (seq_t)exp * ap;
    }

    return result;
}
