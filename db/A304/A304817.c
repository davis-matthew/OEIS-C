/*
 * A304817  Number of divisors of n that are either 1 or not a perfect power.
 * https://oeis.org/A304817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive divisors of n that are either 1 or not
 *            a perfect power (i.e., not of the form x^k with x>1 and k>1)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   compare_pow, isPerfectPower
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/compare_pow.h"
#include "../utils/isPerfectPower.h"

static void count_rec(const uint64_t *pr, const uint64_t *ex, size_t nPr,
                      size_t idx, uint64_t curr, seq_t *cnt)
{
    if (idx == nPr) {
        if (curr == 1 || !isPerfectPower(curr)) (*cnt)++;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t mult = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        count_rec(pr, ex, nPr, idx + 1, curr * mult, cnt);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    /* Work with uint64_t for factorisation */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    uint64_t temp = nn;

    /* Factorisation by trial division */
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (temp > 1) {
        primes[pcnt] = temp;
        exps[pcnt] = 1;
        ++pcnt;
    }

    seq_t total = 0;
    count_rec(primes, exps, pcnt, 0, 1ULL, &total);
    return total;
}
