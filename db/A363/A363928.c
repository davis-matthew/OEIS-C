/*
 * A363928  Expansion of Sum_{k>0} x^(3*k) / (1 - x^(5*k))^2.
 * https://oeis.org/A363928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * Σ_{d|n, d≡3 (mod 5)} (d + 2)
 * Range:     u8 n <= 962, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Parameters: idx_t n ≥ 1 (offset 1).  Returns sequence value in seq_t. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Compute a(n) for the OEIS sequence A363928 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n = 0, return 0 */

    /* -------- factorisation of n -------- */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    uint64_t temp = (uint64_t)n;

    /* factor 2 */
    if ((temp & 1) == 0) {
        uint64_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        primes[cnt] = 2;
        exps[cnt]   = e;
        ++cnt;
    }

    /* trial division by odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if ((temp % p) == 0) {
            uint64_t e = 0;
            while ((temp % p) == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }
    /* remaining prime factor > sqrt(temp) */
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* -------- generate all divisors in an array -------- */
    /* number of divisors */
    size_t div_cnt = 1;
    for (size_t i = 0; i < cnt; ++i)
        div_cnt *= (exps[i] + 1);

    seq_t *divs = (seq_t *)malloc(div_cnt * sizeof(seq_t));
    if (!divs)
        return 0;   /* allocation failure, unlikely */

    divs[0] = 1;
    size_t cur = 1;   /* current number of populated divisors */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        size_t   old = cur;
        seq_t pow    = 1;

        for (uint64_t k = 1; k <= e; ++k) {
            pow = pow * (seq_t)p;
            for (size_t j = 0; j < old; ++j) {
                divs[cur++] = divs[j] * pow;
            }
        }
    }

    /* -------- accumulate the divisor sum for the formula -------- */
    seq_t sum = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        seq_t d = divs[i];
        if (d % 5 == 3) {
            sum += d + (seq_t)2;
        }
    }

    free(divs);
    return sum / 5;
}
