/*
 * A363890  Sum of divisors of 3*n-1 of form 3*k+2.
 * https://oeis.org/A363890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d | (3*n-1), d ≡ 2 (mod 3)} d
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9894 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Sum of divisors ≡ 2 (mod 3) of m. */
static seq_t sum_special_divisors(uint64_t m)
{
    uint64_t primes[16];
    unsigned  exps[16];
    size_t pcnt = 0;
    uint64_t tmp = m;

    /* Prime factorisation of m */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]  = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt]  = 1;
        ++pcnt;
    }

    /* Enumerate all divisors */
    seq_t divs[256];
    size_t dcnt = 1;
    divs[0] = 1;

    for (size_t pf = 0; pf < pcnt; ++pf) {
        uint64_t p = primes[pf];
        unsigned e = exps[pf];
        size_t olddcnt = dcnt;
        seq_t p_pow = 1;
        for (unsigned k = 1; k <= e; ++k) {
            p_pow = p_pow * (seq_t)p;
            for (size_t i = 0; i < olddcnt; ++i) {
                divs[dcnt++] = divs[i] * p_pow;
            }
        }
    }

    /* Sum divisors congruent to 2 modulo 3 */
    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        if ((divs[i] % 3) == 2) {
            sum += divs[i];
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* behaviour below the offset is irrelevant */

    uint64_t m = 3 * (uint64_t)n - 1;
    return sum_special_divisors(m);
}
