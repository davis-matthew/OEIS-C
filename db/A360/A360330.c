/*
 * A360330  a(n) is the number of divisors of n that have only prime factors
 *          that are not prime-indexed primes.
 * https://oeis.org/A360330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = ∏_{p^e‖n, p not prime‑indexed} (e+1),  a(1)=1 */
/*  (Prime‑indexed means the prime’s position in the list of primes is itself prime.) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* static cache of primes up to the largest required value                     */
static uint64_t *prime_list = NULL;   /* array of primes [0 .. prime_cnt-1]     */
static size_t   prime_cnt  = 0;        /* number of primes                       */
static uint64_t prime_max  = 0;        /* largest prime stored                   */

/* Ensure that all primes ≤ limit are in prime_list.                        */
static void ensure_primes_upto(uint64_t limit)
{
    if (prime_max < limit) {
        uint64_t *new_list = NULL;
        size_t    new_cnt  = 0;
        build_primes_upto(limit, &new_list, &new_cnt);
        free(prime_list);
        prime_list = new_list;
        prime_cnt  = new_cnt;
        prime_max  = limit;
    }
}

/* Return the 1‑based index of a prime p (assumes p is indeed prime).       */
static size_t prime_index(uint64_t p)
{
    ensure_primes_upto(p);
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;               /* lo == number of primes ≤ p */
}

/* Test whether a prime has a prime index.                                 */
static bool is_prime_indexed_prime(uint64_t p)
{
    return is_prime(prime_index(p));   /* idx_t is uint64_t */
}

/* --------------------------------------------------------------------- */
/*                       Core sequence function                          */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined for 0, return 0 to avoid crash */
        return 0;

    idx_t m = n;                  /* working copy of the input          */
    seq_t result = 1;             /* multiplicative identity             */

    /* Factorisation by trial division (efficient enough for the expected
       range of inputs).  For each found prime factor p with exponent e,
       multiply the result by (e+1) only if p is NOT a prime‑indexed prime. */
    while (m > 1) {
        uint64_t p = 2;            /* start with the smallest prime       */
        if ((m & 1ULL) == 0) {     /* even part: factor out 2              */
            uint64_t e = 0;
            while ((m & 1ULL) == 0) {
                m >>= 1;
                ++e;
            }
            if (!is_prime_indexed_prime(p))
                result *= (seq_t)(e + 1);
            continue;
        }

        /* look for odd prime divisors */
        uint64_t d = 3;
        bool factor_found = false;
        for (; d <= m / d; d += 2) {
            if (m % d == 0) {
                p = d;
                factor_found = true;
                break;
            }
        }

        if (!factor_found) {       /* m itself is prime                     */
            p = m;
            m = 1;
        } else {
            /* divide out all copies of p */
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (!is_prime_indexed_prime(p))
                result *= (seq_t)(e + 1);
            continue;
        }

        /* m was a prime left after the division loop                       */
        if (!is_prime_indexed_prime(p))
            result *= (seq_t)(2);   /* exponent 1 ⇒ e+1 = 2 */
    }

    return result;
}
