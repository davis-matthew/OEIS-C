/*
 * A393052  Number of divisors d of n such that d and A276086(d) are not
 *          coprime, where A276086 is the primorial base exp-function.
 * https://oeis.org/A393052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Number of divisors d of n such that d and A276086(d) are
 *            not coprime * Implementation: enumerate the divisors of n,
 *            compute A276086(d) via its mixed‑radix * representation
 *            (primorial base), take the gcd with d, and count the cases where
 *            the * gcd is > 1
 * Range:     u8 n <= 19, u16 n <= 86, u32 n <= 4553, u64 n <= 9661,
 *            u128 n <= 9661, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* External utilities */
#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/gcd.h"          /* gcd */

/* Prime table needed for the mixed‑radix weights */
static uint64_t *prime_table = NULL;
static size_t prime_count = 0;
static bool primes_inited = false;

static void init_prime_table(void)
{
    if (primes_inited) return;

    /* 10000 is enough for the tests and for computing the mixed‑radix weights   */
    const size_t LIMIT = 10000;
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    /* Count primes */
    prime_count = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++prime_count;

    /* Allocate and fill prime array */
    prime_table = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    size_t k = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_table[k++] = i;

    free(is_prime);
    primes_inited = true;
}

/* Compute A276086(d) using its primorial‑base digit expansion.
 * The result is stored in *res. */
static void primorial_exp(uint64_t d, seq_t *res)
{
    if (d <= 1) {
        *res = (seq_t)1;
        return;
    }

    /* Build the array of mixed‑radix weights w[i] = product_{j<i} prime[j]  */
    uint64_t w[128];
    size_t wcnt = 0;
    w[wcnt++] = 1;                 /* w[0] = 1 */
    while (wcnt < prime_count && w[wcnt - 1] <= d / prime_table[wcnt - 1]) {
        w[wcnt] = w[wcnt - 1] * prime_table[wcnt - 1];
        ++wcnt;
    }

    /* Highest index that is needed */
    size_t maxidx = wcnt - 1;
    uint64_t rem = d;
    seq_t result = (seq_t)1;

    for (size_t i = maxidx; ; --i) {
        uint64_t digit = rem / w[i];
        rem %= w[i];
        if (digit) {
            seq_t p = (seq_t)prime_table[i];
            seq_t power = (seq_t)1;
            for (uint64_t k = 0; k < digit; ++k)
                power *= p;
            result *= power;
        }
        if (i == 0) break;
    }
    *res = result;
}

/* Generate all divisors of n and store them in divs[].  The array
 * divs must have size at least 128.  Returns the number of divisors. */
static size_t generate_divisors(uint64_t n, uint64_t divs[128])
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    divs[0] = 1;
    size_t div_count = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t cur = div_count;

        for (size_t j = 0; j < cur; ++j) {
            uint64_t val = divs[j];
            for (uint64_t k = 1; k <= e; ++k) {
                val *= p;
                divs[div_count++] = val;
            }
        }
    }
    return div_count;
}

seq_t a(idx_t n)
{
    init_prime_table();

    uint64_t divs[128];
    size_t dcnt = generate_divisors((uint64_t)n, divs);

    seq_t count = 0;

    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        if (d <= 1) continue;               /* 1 never contributes */

        seq_t A;
        primorial_exp(d, &A);

        seq_t g = gcd((seq_t)d, A);
        if (g != (seq_t)1)
            ++count;
    }
    return count;
}
