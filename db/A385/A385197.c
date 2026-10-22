/*
 * A385197  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is a unitary divisor of n is a noncomposite
 *          number.
 * https://oeis.org/A385197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Π_{p^e∥n} (1 - 1/p^e) * (1 + Σ_{p∥n} 1/(p-1))
 * Range:     u8 n <= 194, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It counts, for each k ≤ n, the largest divisor of k that is a unitary divisor of n
   which is a noncomposite number. */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    // factorization containers (max distinct primes for 64-bit n < 64)
    idx_t primes[64];
    int  exps[64];
    int  pcnt = 0;

    idx_t temp = n;
    // simple trial division
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (temp > 1) {          // remaining prime factor
        primes[pcnt] = temp;
        exps[pcnt] = 1;
        ++pcnt;
    }

    // compute base = n * Π (p^e - 1) / p^e  (integer division guaranteed)
    seq_t base = (seq_t)n;
    for (int i = 0; i < pcnt; ++i) {
        // compute p^e
        seq_t powp = 1;
        for (int k = 0; k < exps[i]; ++k)
            powp *= (seq_t)primes[i];

        // division first, then multiplication to avoid overflow and keep integers
        base = base / powp * (powp - 1);
    }

    // result = base + Σ_{p with exponent 1} base / (p - 1)
    seq_t result = base;
    for (int i = 0; i < pcnt; ++i) {
        if (exps[i] == 1) {
            result += base / ((seq_t)primes[i] - 1);
        }
    }
    return result;
}
