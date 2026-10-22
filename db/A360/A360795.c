/*
 * A360795  Expansion of Sum_{k>0} x^k / (1 - (k * x)^k)^(k+1).
 * https://oeis.org/A360795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n-d) * binomial(d + n/d - 1, d)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 27, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

/* Recursive helper to generate all divisors from the prime factorisation */
static void
gen_divs(const idx_t *primes, const idx_t *exps, size_t pcnt,
         idx_t cur, size_t depth, idx_t *divs, size_t *dcnt)
{
    if (depth == pcnt) {
        divs[(*dcnt)++] = cur;
        return;
    }
    idx_t p   = primes[depth];
    idx_t mult = 1;
    for (size_t i = 0; i <= exps[depth]; ++i) {
        gen_divs(primes, exps, pcnt,
                 cur * mult, depth + 1, divs, dcnt);
        mult *= p;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 */

    /* prime factorisation of n */
    idx_t primes[16];
    idx_t exps[16];
    size_t pcnt = 0;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        primes[pcnt] = temp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* generate all divisors of n */
    idx_t divs[2048];
    size_t dcnt = 0;
    gen_divs(primes, exps, pcnt, 1, 0, divs, &dcnt);

    /* sum over all divisors */
    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divs[i];
        seq_t term1 = big_pow((seq_t)d, (seq_t)(n - d));
        seq_t term2 = binomial(d + n / d - 1, d);
        sum += term1 * term2;
    }
    return sum;
}
