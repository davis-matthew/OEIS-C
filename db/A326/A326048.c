/*
 * A326048  a(n) = gcd(n-A050449(n), A082052(n)-n), where A050449 and A082052
 *          give the sum of divisors of the form 4k+1, and not of that form,
 *          respectively.
 * https://oeis.org/A326048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n-A050449(n), A082052(n)-n) where A050449(n) = sum of
 *            divisors of n that are 1 (mod 4) and A082052(n) = sum of all
 *            other divisors of n
 * Range:     u8 n <= 262, u16 n <= 7603, u32 n <= 7603, u64 n <= 7603,
 *            u128 n <= 7603, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include <stdint.h>
#include <stddef.h>

/* factorize n into primes and exponents: primes[], exp[], pcnt */
static void factor_uint64(uint64_t n, uint64_t *primes, int *exp, size_t *pcnt)
{
    *pcnt = 0;
    /* handle factor 2 */
    if ((n & 1ULL) == 0ULL) {
        int e = 0;
        while ((n & 1ULL) == 0ULL) {
            n >>= 1ULL;
            e++;
        }
        primes[(*pcnt)] = 2ULL;
        exp[(*pcnt)] = e;
        (*pcnt)++;
    }
    /* odd factors */
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0ULL) {
            int e = 0;
            while (n % p == 0ULL) {
                n /= p;
                e++;
            }
            primes[(*pcnt)] = p;
            exp[(*pcnt)] = e;
            (*pcnt)++;
        }
    }
    if (n > 1ULL) {           /* remaining prime factor */
        primes[(*pcnt)] = n;
        exp[(*pcnt)] = 1;
        (*pcnt)++;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; undefined below */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[8];
    int exp[8];
    size_t pcnt;
    factor_uint64(nn, primes, exp, &pcnt);

    /* generate all divisors of n */
    uint64_t divs[256];
    size_t dcnt = 1;
    divs[0] = 1ULL;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        int e = exp[i];
        size_t cur = dcnt;
        for (size_t j = 0; j < cur; ++j) {
            uint64_t base = divs[j];
            uint64_t mult = 1ULL;
            for (int k = 1; k <= e; ++k) {
                mult *= p;
                divs[dcnt++] = base * mult;
            }
        }
    }

    /* sum divisors by residue modulo 4 */
    seq_t sum1 = 0;          /* 1 (mod 4) */
    seq_t sumNot1 = 0;       /* otherwise */

    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        if ((d & 3ULL) == 1ULL) {
            sum1 += (seq_t)d;
        } else {
            sumNot1 += (seq_t)d;
        }
    }

    /* absolute differences */
    seq_t diff1;
    seq_t diff2;

    if ((seq_t)nn >= sum1) diff1 = (seq_t)nn - sum1;
    else                   diff1 = sum1 - (seq_t)nn;

    if (sumNot1 >= (seq_t)nn) diff2 = sumNot1 - (seq_t)nn;
    else                      diff2 = (seq_t)nn - sumNot1;

    return gcd(diff1, diff2);
}
