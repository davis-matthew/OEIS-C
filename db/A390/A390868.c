/*
 * A390868  The number of integers k from 1 to n such that gcd(n, k) is a
 *          non-bi-unitary divisor of n.
 * https://oeis.org/A390868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n, d non‑bi‑unitary} φ(n/d)
 * Range:     u8 n <= 683, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define MAX_FACTORS 10

static void
enum_divs(int               idx,
          uint64_t          d,
          const uint64_t   *primes,
          const int        *exps,
          int               pcnt,
          seq_t            *sum,
          int               *ai,
          uint64_t           n)
{
    if (idx == pcnt) {
        bool non = false;
        for (int j = 0; j < pcnt; ++j) {
            if ( (exps[j] & 1) == 0 && ai[j] == exps[j]/2 ) {
                non = true;
                break;
            }
        }
        if (non) {
            uint64_t m = n / d;
            seq_t phi = (seq_t)m;
            for (int j = 0; j < pcnt; ++j) {
                if (ai[j] < exps[j]) {
                    phi -= phi / (seq_t)primes[j];
                }
            }
            *sum += phi;
        }
        return;
    }

    const uint64_t p = primes[idx];
    uint64_t mult = 1;
    for (uint64_t exp = 0; exp <= exps[idx]; ++exp) {
        ai[idx] = (int)exp;
        enum_divs(idx + 1, d * mult, primes, exps, pcnt, sum, ai, n);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint64_t tmp = n;
    uint64_t primes[MAX_FACTORS];
    int exps[MAX_FACTORS];
    int pcnt = 0;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]  = (int)e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt]  = 1;
        ++pcnt;
    }

    seq_t sum = 0;
    int ai[MAX_FACTORS] = {0};
    enum_divs(0, 1, primes, exps, pcnt, &sum, ai, n);
    return sum;
}
