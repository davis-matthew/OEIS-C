/*
 * A337331  Number of pairs of divisors of n, (d1,d2), with d1 <= d2, whose
 *          average is an integer and divides n.
 * https://oeis.org/A337331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d1|n, d2|n, d1<=d2, (d1+d2)|(2*n), 2|(d1+d2)} 1
 * Range:     u8 n <= 5039, u16 n <= 9232, u32 n <= 9232, u64 n <= 9232,
 *            u128 n <= 9232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void generate_divisors(uint64_t idx, uint64_t current,
                              uint64_t *divs, size_t *pos,
                              const uint64_t *primes, const unsigned *exps,
                              size_t pcnt)
{
    if (idx == pcnt) {
        divs[(*pos)++] = current;
        return;
    }
    for (unsigned k = 0; k <= exps[idx]; ++k) {
        generate_divisors(idx + 1, current, divs, pos, primes, exps, pcnt);
        current *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    /* factor n */
    const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    unsigned exps[MAX_FACTORS];
    size_t pcnt = 0;
    uint64_t tmp = (uint64_t)n;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* compute total number of divisors */
    size_t dcnt = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        dcnt *= (exps[i] + 1);
    }

    /* generate all divisors */
    uint64_t *divs = (uint64_t *)malloc(dcnt * sizeof(uint64_t));
    size_t pos = 0;
    generate_divisors(0, 1, divs, &pos, primes, exps, pcnt);
    /* pos should equal dcnt */

    /* count qualifying pairs */
    seq_t ans = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d1 = divs[i];
        for (size_t j = i; j < dcnt; ++j) {
            uint64_t d2   = divs[j];
            uint64_t sum  = d1 + d2;
            if ((sum & 1ULL) == 0) { /* sum is even */
                uint64_t mod = (uint64_t)n % sum;
                if ( ((mod << 1) % sum) == 0 ) {
                    ++ans;
                }
            }
        }
    }

    free(divs);
    return ans;
}
