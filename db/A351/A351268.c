/*
 * A351268  Sum of the 5th powers of the squarefree divisors of n.
 * https://oeis.org/A351268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p|n} (1 + p^5)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 84, u64 n <= 7085,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but safe */
    uint64_t m = (uint64_t)n;
    seq_t res = 1;
    if (m <= 1) return res;

    /* handle prime factor 2 */
    if ((m & 1) == 0) {
        seq_t p = 2;
        seq_t p5 = (((((p * p) * p) * p) * p));
        res *= ((seq_t)1 + p5);
        while ((m & 1) == 0) m >>= 1;
    }

    /* odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            seq_t pp = (seq_t)p;
            seq_t p5 = (((((pp * pp) * pp) * pp) * pp));
            res *= ((seq_t)1 + p5);
            while (m % p == 0) m /= p;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1) {
        seq_t pp = (seq_t)m;
        seq_t p5 = (((((pp * pp) * pp) * pp) * pp));
        res *= ((seq_t)1 + p5);
    }

    return res;
}
