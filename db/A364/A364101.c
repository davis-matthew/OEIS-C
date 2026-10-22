/*
 * A364101  Sum of divisors of 5*n-2 of form 5*k+4.
 * https://oeis.org/A364101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula:
   a(n) = sum of all divisors d of (5*n-2) such that d mod 5 = 4
   Offset: 1   (a(1) is the first term) */

#include <stddef.h>
#include <stdint.h>

/* Back‑track over the prime factorisation of a number.  `factors` and `exps`
   contain the distinct prime factors of the number and their exponents.
   `idx_t` is an unsigned integer large enough for the factoured value. */
static void dfs_divisors(const idx_t *factors,
                         const uint8_t *exps,
                         size_t count,
                         idx_t current,
                         seq_t *sum)
{
    if (count == 0) {
        /* reached the end of prime factors */
        if ((current % (idx_t)5U) == (idx_t)4U)
            *sum += (seq_t)current;
        return;
    }
    /* We are at `factors[count-1]`; process all exponents for it. */
    const idx_t prime = factors[count-1];
    const uint8_t exp   = exps[count-1];
    idx_t prod = 1U;
    for (uint8_t k = 0; k <= exp; ++k) {
        dfs_divisors(factors, exps, count-1, current * prod, sum);
        prod *= prime;
    }
}

/* Euler's totient is not needed here, but this helper makes the code
   easier to read. */
seq_t a(idx_t n)
{
    /* Compute m = 5*n - 2 in the index type to avoid overflow. */
    idx_t m = 5U * n - (idx_t)2U;
    if (m <= (idx_t)0)
        return (seq_t)0;

    /* Factorisation of m into distinct prime factors and exponents. */
    idx_t primes[64];
    uint8_t exps[64];
    size_t count = 0;

    idx_t temp = m;
    idx_t p = 2U;
    while (p * p <= temp) {
        if (temp % p == (idx_t)0U) {
            uint8_t e = 0U;
            while (temp % p == (idx_t)0U) {
                temp /= p;
                ++e;
            }
            primes[count] = p;
            exps[count]   = e;
            ++count;
        }
        /* Move to the next candidate prime. */
        p = (p == (idx_t)2U) ? (idx_t)3U : p + 2U;
    }
    if (temp > (idx_t)1U) {          /* Remaining prime factor > sqrt(m) */
        primes[count] = temp;
        exps[count]   = 1U;
        ++count;
    }

    /* Enumerate all divisors and sum those ≡ 4 (mod 5). */
    seq_t sum = (seq_t)0;
    dfs_divisors(primes, exps, count, (idx_t)1U, &sum);
    return sum;
}
