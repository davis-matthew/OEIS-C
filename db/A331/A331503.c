/*
 * A331503  a(n) is the number of sets modulo n which can be formed by a
 *          finite arithmetic sequence.
 * https://oeis.org/A331503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) + n*(tau(n) - 1 - 3*floor(n/2) +
 *            Sum_{i=1..floor(n/2)} n/gcd(n,i))
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 2052, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

static void factor(uint64_t n, uint64_t primes[64], int exps[64], size_t *cnt) {
    size_t c = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[c] = p;
            exps[c]   = e;
            ++c;
        }
    }
    if (m > 1) {
        primes[c] = m;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n) {
    /* sigma(n) and tau(n) from prime factorization */
    uint64_t primes[64];
    int exps[64];
    size_t cnt;
    factor(n, primes, exps, &cnt);

    seq_t sigma = 1;
    seq_t tau   = 1;
    for (size_t idx = 0; idx < cnt; ++idx) {
        seq_t p = (seq_t)primes[idx];
        int e   = exps[idx];

        /* compute p^(e+1) */
        seq_t pe = 1;
        for (int i = 0; i < e + 1; ++i) pe *= p;

        seq_t term = pe - (seq_t)1;
        term /= (p - (seq_t)1);
        sigma *= term;
        tau   *= (e + 1);
    }

    idx_t half = n / 2;
    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;
    for (idx_t i = 1; i <= half; ++i) {
        seq_t g = gcd(n_seq, (seq_t)i);
        sum += n_seq / g;
    }

    seq_t inner = tau - (seq_t)1;
    inner -= (seq_t)(3 * half);
    inner += sum;

    return sigma + n_seq * inner;
}
