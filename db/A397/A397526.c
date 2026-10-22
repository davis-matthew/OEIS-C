/*
 * A397526  Number of ways to write 6*n as p + q, where p and q are primes
 *          such that 2*p+1 and 4*q+1 are also prime.
 * https://oeis.org/A397526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = number of pairs (p,q) of primes with * p + q = 6*n, * 2*p
 *            + 1 and 4*q + 1 are also prime. * a(1) = 1, a(2) = 1, a(3) = 2,
 *            … * (OEIS A397526)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- cached sieve data --------------------------------------------*/
static size_t prime_limit      = 0;    /* furthest index sieved */
static bool *prime_flags       = NULL; /* prime_flags[i] == true if i is prime */
static uint32_t *prime_list    = NULL; /* list of primes found so far */
static size_t prime_count      = 0;    /* number of primes in prime_list */

/* ensure all primes up to 'limit' are in prime_flags and prime_list */
static void ensure_sieve(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* allocate new sieve array */
    size_t new_limit = (size_t)limit;
    bool *flags = (bool*)calloc(new_limit + 1, sizeof(bool));
    if (!flags) return;  /* allocation failure – skip */

    /* simple Sieve of Eratosthenes */
    for (size_t i = 2; i <= new_limit; ++i)
        flags[i] = true;
    for (size_t i = 2; (size_t)i * i <= new_limit; ++i) {
        if (flags[i]) {
            for (size_t j = i * i; j <= new_limit; j += i)
                flags[j] = false;
        }
    }

    /* count primes to size the list */
    size_t cnt = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (flags[i]) ++cnt;

    uint32_t *list = (uint32_t*)realloc(prime_list, cnt * sizeof(uint32_t));
    if (!list) { free(flags); return; }   /* allocation failure */
    prime_list = list;
    prime_count = 0;
    for (size_t i = 2; i <= new_limit; ++i)
        if (flags[i]) prime_list[prime_count++] = (uint32_t)i;

    /* replace old flags */
    free(prime_flags);
    prime_flags = flags;
    prime_limit = new_limit;
}

/* primality test using the cached sieve */
static inline bool is_prime(idx_t n)
{
    return n <= prime_limit ? prime_flags[(size_t)n] : false;
}

/* p satisfies: p is prime, and (p==2 or p==3 or (p%3==2 and 2*p+1 is prime)) */
static inline bool valid_p(idx_t p)
{
    if (!is_prime(p)) return false;
    if (p == 2 || p == 3) return true;
    if ((p % 3) != 2) return false;
    return is_prime(2 * p + 1);
}

/* q satisfies: q is prime, and (q==3 or (q%3==1 and 4*q+1 is prime)) */
static inline bool valid_q(idx_t q)
{
    if (!is_prime(q)) return false;
    if (q == 3) return true;                    /* 4*3+1 = 13 prime */
    if (q == 2) return false;                   /* 4*2+1 = 9 not prime */
    if ((q % 3) != 1) return false;
    return is_prime(4 * q + 1);
}

/* main sequence function --------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for offset 1 */

    idx_t m = 6 * n;                          /* target sum p + q */
    idx_t need = 4 * m + 1;                   /* max of 4*q+1 needed */
    ensure_sieve(need);

    seq_t cnt = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > m) break;
        if (!valid_p(p)) continue;
        idx_t q = m - p;
        if (!valid_q(q)) continue;
        ++cnt;
    }
    return cnt;
}
