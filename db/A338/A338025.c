/*
 * A338025  a(n) = LCM(k*j_1!*...*j_k! : j_1,...,j_k>=1, j_1+...+j_k=n,
 *          k=1,...,n)/n!.
 * https://oeis.org/A338025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM(k * j1!*…*jk! : j1,…,jk≥1, j1+…+jk=n, k=1,…,n) / n!
 *            Equivalently, for each prime p≤n the exponent of p in a(n) is
 *            the largest integer t with p^t ≤ S_p(n), where S_p(n) is the sum
 *            of the base‑p digits of n. The following implementation follows
 *            this description
 * Range:     u8 n <= 20, u16 n <= 60, u32 n <= 180, u64 n <= 420,
 *            u128 n <= 1800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* cache of primes up to the largest n requested so far */
static uint64_t *primes = NULL;
static size_t    prime_cnt = 0;
static idx_t    primes_up_to = 0;

/* Ensure we have a list of all primes ≤ n.  The classic sieve is used
   internally, and the allocated array is freed/recreated on each request
   for a larger n.  */
static void ensure_primes(idx_t n)
{
    if (n <= primes_up_to)
        return;

    if (primes) {
        free(primes);
        primes = NULL;
    }

    idx_t limit = n;
    /* allocate one byte per integer up to limit – 0 means prime, 1 composite */
    uint8_t *composite = (uint8_t *)malloc((size_t)(limit + 1));
    if (!composite)
        exit(EXIT_FAILURE);

    memset(composite, 0, (size_t)(limit + 1));

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                composite[j] = 1;
        }
    }

    /* Count primes to allocate the array */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!composite[i])
            ++cnt;

    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes)
        exit(EXIT_FAILURE);

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!composite[i])
            primes[idx++] = (uint64_t)i;

    prime_cnt    = cnt;
    primes_up_to = limit;
    free(composite);
}

seq_t a(idx_t n)
{
    if (n == 0)                /* offset starts at 1 – guard for safety */
        return 0;

    ensure_primes(n);

    seq_t result = 1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        if (p > n)
            break;

        /* sum of base‑p digits of n */
        idx_t   tmp = n;
        seq_t   s   = 0;
        while (tmp > 0) {
            s += (seq_t)(tmp % p);
            tmp /= p;
        }

        /* largest power of p not exceeding s */
        seq_t power = 1;
        seq_t bigp  = (seq_t)p;
        while (power * bigp <= s) {
            power *= bigp;
        }

        result *= power;
    }

    return result;
}
