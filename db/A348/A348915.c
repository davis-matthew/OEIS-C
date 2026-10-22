/*
 * A348915  a(n) = Sum_{d|n} d^(d mod 2).
 * https://oeis.org/A348915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(d mod 2) (add odd divisors, add 1 for even
 *            divisors)
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t (used for bitwise ops) */

seq_t a(idx_t n) {
    /* Find the exponent of 2 in n (k) and the odd part m = n / 2^k */
    idx_t k = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        k++;
        tmp >>= 1;
    }
    seq_t m = (seq_t)tmp;   /* odd part, fits in seq_t */

    /* σ(m) (sum of divisors of m) and τ(m) (number of divisors of m) */
    seq_t sigma = 1;
    seq_t tau   = 1;

    if (m > 1) {
        idx_t mm = (idx_t)m;          /* mutable copy for factoring */
        for (idx_t p = 3; p * p <= mm; p += 2) {
            if (mm % p == 0) {
                idx_t exp = 0;
                while (mm % p == 0) {
                    mm /= p;
                    exp++;
                }
                /* Compute (p^(exp+1) - 1) / (p - 1) using integer arithmetic */
                seq_t power = 1;
                for (idx_t i = 0; i < exp + 1; ++i) {
                    power *= (seq_t)p;
                }
                seq_t term = (power - 1) / (seq_t)(p - 1);
                sigma *= term;
                tau   *= (seq_t)(exp + 1);
            }
        }
        if (mm > 1) { /* remaining prime factor */
            sigma *= ((seq_t)mm + 1);
            tau   *= 2;
        }
    }

    /* a(n) = σ(m) + k * τ(m)   (k=0 gives σ(m) directly) */
    seq_t result = sigma;
    if (k > 0) {
        result += (seq_t)k * tau;
    }
    return result;
}
