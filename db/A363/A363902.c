/*
 * A363902  Expansion of Sum_{k>0} x^(2*k) / (1 - x^(3*k))^2.
 * https://oeis.org/A363902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3) * Sum_{d|n, d % 3 == 2} (d + 1)
 * Range:     u8 n <= 559, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_spf.h"

static idx_t _spf_limit = 0;
static idx_t * _spf = NULL;

static void _ensure_spf(idx_t n)
{
    if (n <= _spf_limit) return;
    if (_spf) free(_spf);
    build_spf(n, &_spf);
    _spf_limit = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    _ensure_spf(n);

    /* Factorize n using the smallest prime factor array */
    idx_t temp = n;
    idx_t primes[32];
    idx_t exps[32];
    int pcnt = 0;
    while (temp > 1) {
        idx_t p = _spf[temp];
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        primes[pcnt] = p;
        exps[pcnt] = e;
        ++pcnt;
    }

    /* Generate all divisors of n */
    idx_t divs[256];
    int dcnt = 1;
    divs[0] = 1;
    for (int i = 0; i < pcnt; ++i) {
        idx_t p = primes[i];
        idx_t e = exps[i];
        int oldcnt = dcnt;
        for (int j = 0; j < oldcnt; ++j) {
            idx_t base = divs[j];
            idx_t power = 1;
            for (int t = 1; t <= e; ++t) {
                power *= p;
                divs[dcnt++] = base * power;
            }
        }
    }

    /* Sum over divisors congruent to 2 mod 3 */
    seq_t sum = 0;
    for (int i = 0; i < dcnt; ++i) {
        if (divs[i] % 3 == 2) {
            sum += (seq_t)divs[i] + 1;
        }
    }

    return sum / 3;
}
