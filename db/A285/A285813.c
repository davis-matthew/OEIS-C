/*
 * A285813  Let p_1<p_2<... be primes such that 2*n-p_i is prime. a(n) is the
 *          smallest i such that 2*n+(odd part of p_i-1) is prime or a(n)=0 if
 *          there is no such i.
 * https://oeis.org/A285813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest i such that 2*n–p_i is prime and *
 *            2*n+odd_part(p_i–1) is prime, otherwise 0
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* primality test for seq_t */

/* --- Dynamic prime sieve: prime_is[x] == true iff x is prime.  --- */
static bool *prime_is = NULL;      /* byte per integer */
static idx_t sieve_limit = 0;      /* highest number currently sieved */

/* Ensure that all integers up to 'limit' are marked for primality. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= sieve_limit) return;           /* already sufficient */

    free(prime_is);
    prime_is = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!prime_is) return;                      /* memory failure */

    for (idx_t i = 0; i <= limit; ++i) prime_is[i] = true;
    prime_is[0] = prime_is[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (prime_is[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_is[j] = false;
        }
    }
    sieve_limit = limit;
}

/* Compute the odd part of an integer: remove all factors of 2. */
static seq_t odd_part_uint64(uint64_t x)
{
    seq_t r = (seq_t)x;
    while ((r & 1) == 0) r >>= 1;
    return r;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined below offset */

    idx_t two_n = n + n;                 /* 2*n */
    ensure_primes_upto(two_n);           /* need prime test for 2*n - p */

    /* i counts indices in the filtered subsequence of primes where
     * 2*n - p is prime. It starts at 1. */
    seq_t i = 1;

    for (idx_t p = 2; p < two_n; ++p) {
        if (!prime_is[p]) continue;      /* p must be prime */

        idx_t q = two_n - p;
        if (q < 2 || !prime_is[q]) continue;   /* 2*n-p must be prime */

        seq_t odd = odd_part_uint64(p - 1);
        seq_t candidate = (seq_t)two_n + odd;

        if (is_prime_big(candidate))
            return i;                     /* first suitable i found */

        ++i;                              /* advance filtered index */
    }
    return 0;                              /* no suitable prime */
}
