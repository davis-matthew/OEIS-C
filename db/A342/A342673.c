/*
 * A342673  a(n) = gcd(n, sigma(A003961(n))), where A003961 is fully
 *          multiplicative with a(prime(k)) = prime(k+1), and sigma is the sum
 *          of divisors of n.
 * https://oeis.org/A342673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, sigma(A003961(n)))
 * Range:     u8 n <= 350, u16 n <= 9247, u32 n <= 9247, u64 n <= 9247,
 *            u128 n <= 9247, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/*  Prime list used for factorisation and for “next prime’’ lookup   */
#define PRIME_MAX 20000   /*  covers all primes needed for n<=10000  */
static idx_t primes[2500];
static size_t prime_cnt = 0;
static bool primes_initialised = false;

/*  Initialise the prime table once.  Uses a simple sieve of Eratosthenes.  */
static void init_primes(void)
{
    if (primes_initialised) return;
    primes_initialised = true;

    unsigned char sieve[PRIME_MAX + 1];
    for (idx_t i = 2; i <= PRIME_MAX; ++i) sieve[i] = 1;

    for (idx_t p = 2; p * p <= PRIME_MAX; ++p) {
        if (sieve[p]) {
            for (idx_t q = p * p; q <= PRIME_MAX; q += p) sieve[q] = 0;
        }
    }

    for (idx_t i = 2; i <= PRIME_MAX; ++i)
        if (sieve[i]) primes[prime_cnt++] = i;
}

/*  Return the index of prime p in the prime list.  Assumes p is prime
    and exists in the list; returns SIZE_MAX on failure.           */
static size_t prime_index(idx_t p)
{
    for (size_t i = 0; i < prime_cnt; ++i)
        if (primes[i] == p) return i;
    return (size_t)-1;   /* should not happen for n≤10000  */
}

/*  Compute a(n) as per the definition.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0, harmless */

    init_primes();                /* ensure prime list ready */

    uint64_t sigma_mod = 1;       /* sigma(A003961(n)) modulo n   */
    idx_t temp = n;               /* material to be factorised    */

    /*  Loop over primes up to sqrt(temp)  */
    for (size_t i = 0; i < prime_cnt; ++i) {
        const idx_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            /* count exponent e of p in n  */
            uint32_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* next prime after p  */
            size_t idx = prime_index(p);
            if (idx == (size_t)-1 || idx + 1 >= prime_cnt) return 0; /* safe guard */
            const idx_t np = primes[idx + 1];

            /* compute sigma(np^e) = (np^{e+1} - 1)/(np - 1)  */
            uint64_t pow_np = 1;
            for (uint32_t k = 0; k < e + 1; ++k) pow_np *= np;
            uint64_t term = (pow_np - 1) / (np - 1);
            term %= n;                     /* reduce modulo n   */
            sigma_mod = (sigma_mod * term) % n;
        }
    }

    /*  Remaining prime factor > sqrt(n)  */
    if (temp > 1) {
        idx_t p = temp;               /* this is prime */
        size_t idx = prime_index(p);
        if (idx == (size_t)-1 || idx + 1 >= prime_cnt) return 0; /* safe guard */
        const idx_t np = primes[idx + 1];

        uint64_t term = (np * np - 1) / (np - 1);   /* e=1  */
        term %= n;
        sigma_mod = (sigma_mod * term) % n;
    }

    /*  Return gcd(n, sigma_mod)  */
    return gcd((seq_t)n, (seq_t)sigma_mod);
}
