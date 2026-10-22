/*
 * A381202  a(n) is the sum of the elements of the set of bases and exponents
 *          (including exponents = 1) in the prime factorization of n.
 * https://oeis.org/A381202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of all distinct primes and exponents in the prime
 *            factorization of n (including exponent 1 when it appears). The
 *            empty set for n = 1 yields a(1) = 0
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* n = 1 => empty factorization */
    if (n <= 1)
        return 0;

    /* factorization arrays (max 64 distinct primes for 64‑bit numbers) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t fc = 0;          /* number of distinct prime factors */

    uint64_t m = (uint64_t)n;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            uint64_t e = 0;
            while (m % d == 0) {
                m /= d;
                ++e;
            }
            primes[fc] = d;
            exps[fc]  = e;
            ++fc;
        }
    }
    if (m > 1) {           /* remaining prime factor */
        primes[fc] = m;
        exps[fc]   = 1;
        ++fc;
    }

    /* collect distinct values (primes and exponents) */
    seq_t uniq[128];
    size_t uniqc = 0;

    for (size_t i = 0; i < fc; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];

        /* add prime if not already present */
        int found = 0;
        for (size_t k = 0; k < uniqc; ++k)
            if (uniq[k] == p) { found = 1; break; }
        if (!found) uniq[uniqc++] = p;

        /* add exponent if not already present */
        found = 0;
        for (size_t k = 0; k < uniqc; ++k)
            if (uniq[k] == e) { found = 1; break; }
        if (!found) uniq[uniqc++] = e;
    }

    /* sum the distinct values */
    seq_t sum = 0;
    for (size_t i = 0; i < uniqc; ++i)
        sum += uniq[i];

    return sum;
}
