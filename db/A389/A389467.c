/*
 * A389467  Number of natural numbers in range [n .. A003961(n)-1] that are
 *          coprime to n.
 * https://oeis.org/A389467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=n..A003961(n)-1} [gcd(k,n)=1], computed as
 *            C(A003961(n)-1) - C(n-1), where C(X) = Sum_{d|rad(n)}
 *            mu(d)*floor(X/d) counts 1<=k<=X coprime to n
 * Range:     u8 n <= 47, u16 n <= 2047, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 100 terms through n = 9411 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

static seq_t A389467_coprime_upto(seq_t X, const uint64_t *pr, int pc)
{
    seq_t pos = 0, neg = 0;
    int subsets = 1 << pc;
    for (int mask = 0; mask < subsets; ++mask) {
        seq_t prod = 1;
        int bits = 0;
        for (int i = 0; i < pc; ++i) {
            if (mask & (1 << i)) {
                prod *= (seq_t)pr[i];
                ++bits;
            }
        }
        seq_t term = X / prod;
        if (bits & 1) neg += term; else pos += term;
    }
    return pos - neg;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    uint64_t pr[24];
    int pc = 0;
    uint64_t m = (uint64_t)n;
    seq_t A = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            pr[pc++] = p;
            while (m % p == 0) {
                m /= p;
                A *= (seq_t)q;
            }
        }
    }
    if (m > 1) {
        pr[pc++] = m;
        A *= (seq_t)next_prime(m);
    }
    seq_t hi = A389467_coprime_upto(A - 1, pr, pc);
    seq_t lo = A389467_coprime_upto((seq_t)n - 1, pr, pc);
    return hi - lo;
}
