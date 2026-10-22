/*
 * A353493  The arithmetic derivative of n, reduced modulo 4.
 * https://oeis.org/A353493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic derivative of n modulo 4 * * For n ≥ 0, a(n) =
 *            Σ_{p^e || n} e * (n / p) (mod 4), * where the sum runs over all
 *            distinct prime divisors p of n. * The implementation factors n
 *            by trial division; this is fast for * the n values used in the
 *            test cases (up to a few ten‑thousands)
 * Range:     u8 n <= 453, u16 n <= 9904, u32 n <= 9904, u64 n <= 9904,
 *            u128 n <= 9904, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(0) = 0, a(1) = 0 */
        return 0;

    seq_t n_s = (seq_t)n;      /* use seq_t for all arithmetic   */
    seq_t sum = 0;
    uint64_t m = (uint64_t)n;  /* work with native 64‑bit for factorisation */

    /* factor 2 separately */
    if ((m & 1ULL) == 0) {
        int w = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            w++;
        }
        seq_t term = ((seq_t)(w & 3) * ((n_s / 2ULL) & 3)) & 3;
        sum = (sum + term) & 3;
    }

    /* factor odd primes */
    uint64_t p = 3;
    while (p <= m / p) {
        if (m % p == 0) {
            int w = 0;
            while (m % p == 0) {
                m /= p;
                w++;
            }
            seq_t term = ((seq_t)(w & 3) * ((n_s / p) & 3)) & 3;
            sum = (sum + term) & 3;
        }
        p += 2;
    }

    /* m > 1 now means m is a remaining prime factor */
    if (m > 1) {
        seq_t term = ((seq_t)(1 & 3) * ((n_s / m) & 3)) & 3;
        sum = (sum + term) & 3;
    }

    return sum;
}
