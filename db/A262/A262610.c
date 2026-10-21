/*
 * A262610  The values of sigma function (A000203) and the positive integers
 *          interleaved.
 * https://oeis.org/A262610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k-1) = σ(k) (sum of divisors of k) * a(2k) = k * where σ is
 *            Euler's divisor-sum function. * * Computes the terms of OEIS
 *            A262610 without precomputation. * The divisor sum is calculated
 *            by prime factorisation using trial division. * This is efficient
 *            for n up to about 10 000 (k ≤ 5 000)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Even index: return n/2. */
    if ((n & 1ULL) == 0) {
        return (seq_t)(n >> 1);      /* n/2 */
    }

    /* Odd index: compute σ((n+1)/2). */
    idx_t k = (n + 1) >> 1;          /* (n+1)/2 */
    seq_t sigma_k = 1;
    idx_t m = k;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t sum = 1;          /* sum of powers of p: 1 + p + ... + p^e */
            seq_t pow = 1;
            while (m % p == 0) {
                m /= p;
                pow *= (seq_t)p;
                sum += pow;
            }
            sigma_k *= sum;
        }
    }

    /* If remaining factor is prime. */
    if (m > 1) {
        sigma_k *= ((seq_t)m + 1);  /* 1 + m */
    }

    return sigma_k;
}
