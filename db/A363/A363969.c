/*
 * A363969  Expansion of Sum_{k>0} k^2 * x^(2*k-1) / (1 - x^(2*k-1)).
 * https://oeis.org/A363969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d odd} ((d+1)/2)^2
 * Range:     u8 n <= 30, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Recursive helper that accumulates the sum of terms for all odd divisors. */
static void sumDivisorsRec(size_t idx, idx_t cur, const uint64_t *primes,
                           const uint8_t *exps, size_t num, seq_t *sum) {
    if (idx == num) {
        seq_t d = (seq_t)cur;          /* odd divisor */
        seq_t half = (d + 1) / 2;      /* (d+1)/2  */
        *sum += half * half;           /* add ((d+1)/2)^2 */
        return;
    }
    uint64_t p = primes[idx];
    uint8_t e = exps[idx];
    idx_t val = cur;
    for (uint8_t k = 0; k <= e; ++k) {
        sumDivisorsRec(idx + 1, val, primes, exps, num, sum);
        if (k < e)
            val = val * (idx_t)p;     /* multiply by p for next exponent */
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset starts at 1, but guard for 0 */

    /* Remove all factors of 2 – even divisors are irrelevant. */
    idx_t m = n;
    while ((m & 1) == 0) m >>= 1;

    /* Factorise the odd part m */
    uint64_t primes[64];
    uint8_t exps[64];
    size_t num = 0;
    idx_t tmp = m;

    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint8_t e = 0;
            do {
                tmp /= p;
                ++e;
            } while (tmp % p == 0);
            primes[num] = (uint64_t)p;
            exps[num] = e;
            ++num;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        primes[num] = (uint64_t)tmp;
        exps[num] = 1;
        ++num;
    }

    seq_t sum = 0;
    sumDivisorsRec(0, (idx_t)1, primes, exps, num, &sum);
    return sum;
}
