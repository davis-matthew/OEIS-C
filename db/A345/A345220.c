/*
 * A345220  Number of divisors d of n with an even number of primes not
 *          exceeding d.
 * https://oeis.org/A345220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} ((pi(d)+1) mod 2) a(n) counts divisors d of n
 *            for which pi(d) is even
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   count_even_pi_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for the defined types and memory allocation. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/count_even_pi_divisors.h"

/* The type definitions seq_t and idx_t are provided by the build system. */
/* We use idx_t for all integer calculations that may exceed 32 bits,
   and seq_t for the result, which is an unsigned integer type chosen
   by the build (e.g., uint8_t, uint64_t, unsigned __int128, etc.). */

/* Forward declaration of the helper that enumerates divisors. */
static seq_t count_even_pi_divisors(const idx_t *pr, const idx_t *exp,
                                    size_t cnt, size_t pos, idx_t cur,
                                    const idx_t *pi_prefix);

/* ----------------------------------------------------------------------- */
/* Public function required by the framework.                           ----- */
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Return 0 for an invalid index; the sequence is defined for n>=1. */
    if (n == 0) return 0;

    /* --------------------------------------------------------------- */
    /* Build the prime-count prefix array pi_prefix[0..n]               */
    /* --------------------------------------------------------------- */
    const idx_t limit = n;
    /* Boolean array for the sieve (using idx_t as 0/1).  */
    idx_t *is_prime = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!is_prime) return 0;          /* allocation failure */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }
    idx_t *pi_prefix = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!pi_prefix) {
        free(is_prime);
        return 0;
    }
    idx_t acc = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        if (is_prime[i]) ++acc;
        pi_prefix[i] = acc;
    }
    free(is_prime);

    /* --------------------------------------------------------------- */
    /* Factor n into distinct primes and their exponents.                */
    /* --------------------------------------------------------------- */
    idx_t primes[16];      /* fits the maximum number of distinct factors for n<=10^4 */
    idx_t exps  [16];
    size_t cnt = 0;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]  = e;
            ++cnt;
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* --------------------------------------------------------------- */
    /* Enumerate all divisors and count those with even pi(d).           */
    /* --------------------------------------------------------------- */
    seq_t result = count_even_pi_divisors(primes, exps, cnt, 0, (idx_t)1, pi_prefix);

    free(pi_prefix);
    return result;
}

/* ----------------------------------------------------------------------- */
/* Helper: recursively enumerate divisors of the factored number.          */
/* Carried out with the global prime-prefix array for pi(d).               */
