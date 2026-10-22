/*
 * A348998  a(n) = A348928(A276086(n)), where A348928(n) = gcd(n, A003958(n)),
 *          and A003958 is multiplicative with a(p^e) = (p-1)^e, and A276086
 *          gives the prime product form of primorial base expansion of n.
 * https://oeis.org/A348998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = gcd(A276086(n), A003958(A276086(n)))
 * Range:     u8 n <= 496, u16 n <= 9772, u32 n <= 9772, u64 n <= 9772,
 *            u128 n <= 9772, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Computes the sequence a(n) as defined in OEIS A348998 */
/* A276086(n) is the prime product of the primorial base expansion of n */
/* A003958 is multiplicative with a(p^e) = (p-1)^e */
/* A348928(n) = gcd(n, A003958(n)) */
/* Hence a(n) = gcd( A276086(n), A003958(A276086(n)) ) */

seq_t a(idx_t n)
{
    /* Upper bound for prime generation */
    idx_t limit = n + 1;
    if (limit < 2) limit = 2;

    /* Simple sieve to generate primes up to 'limit' */
    bool *is_comp = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_comp) return (seq_t)0;
    for (idx_t i = 0; i <= limit; ++i) is_comp[i] = false;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!is_comp[p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                is_comp[q] = true;
        }
    }

    /* Collect primes */
    size_t primes_cnt = 0;
    uint64_t *primes = (uint64_t*)malloc((size_t)(limit) * sizeof(uint64_t));
    if (!primes) { free(is_comp); return (seq_t)0; }

    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) primes[primes_cnt++] = (uint64_t)i;

    /* Digits of n in primorial base */
    idx_t *digits = (idx_t*)calloc(primes_cnt, sizeof(idx_t));
    if (!digits) { free(primes); free(is_comp); return (seq_t)0; }

    idx_t temp = n;
    size_t pos = 0;
    while (temp > 0 && pos < primes_cnt) {
        digits[pos] = temp % primes[pos];
        temp /= primes[pos];
        ++pos;
    }
    size_t processed = pos;  /* number of digits actually used */

    /* Compute exponent of each prime in A003958 part */
    idx_t *exp_T = (idx_t*)calloc(primes_cnt, sizeof(idx_t));
    if (!exp_T) { free(digits); free(primes); free(is_comp); return (seq_t)0; }

    for (size_t i = 0; i < processed; ++i) {
        if (digits[i] == 0) continue;
        uint64_t q_minus1 = primes[i] - 1;
        uint64_t m = q_minus1;
        for (size_t f = 0; f < primes_cnt && (uint64_t)primes[f] * (uint64_t)primes[f] <= m; ++f) {
            if (m % primes[f] == 0) {
                idx_t cnt = 0;
                while (m % primes[f] == 0) {
                    m /= primes[f];
                    ++cnt;
                }
                exp_T[f] += digits[i] * cnt;
            }
        }
        if (m > 1) {  /* remaining prime factor */
            /* m must be one of the primes previously generated */
            for (size_t f = 0; f < primes_cnt; ++f) {
                if (primes[f] == m) {
                    exp_T[f] += digits[i];
                    break;
                }
            }
        }
    }

    /* Compute the result as product of primes^{min(digits, exp_T)} */
    seq_t result = 1;
    for (size_t i = 0; i < processed; ++i) {
        idx_t exp_min = digits[i] < exp_T[i] ? digits[i] : exp_T[i];
        if (exp_min == 0) continue;
        for (idx_t k = 0; k < exp_min; ++k) {
            result *= (seq_t)primes[i];
        }
    }

    /* Clean up */
    free(exp_T);
    free(digits);
    free(primes);
    free(is_comp);
    return result;
}
