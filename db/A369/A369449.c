/*
 * A369449  a(n) = A003415(gcd(A003415(n), A276086(n))), where A003415 is the
 *          arithmetic derivative and A276086 is the primorial base
 *          exp-function.
 * https://oeis.org/A369449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(gcd(A003415(n), A276086(n)))
 * Range:     u8 n <= 19, u16 n <= 85, u32 n <= 1589, u64 n <= 9952,
 *            u128 n <= 9952, big any n
 * Helpers:   gcd, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/pow_seq.h"

/* ------------------------------------------------------------------ */
/*  Prime table used by the arithmetic derivative and primorial base  */
/*  exponent function.                                                 */
#define PRIME_LIMIT 50000          /* enough for n up to about 10⁴ */

static bool *prime_sieve = NULL;  /* dynamic sieve array */
static uint32_t *prime_list = NULL;/* list of primes */
static size_t prime_cnt = 0;       /* number of primes in prime_list */

/* initialise the primes array */
static void init_primes(void)
{
    if (prime_sieve != NULL) return;      /* already done */

    /* Sieve of Eratosthenes (bool array) */
    prime_sieve = (bool *)malloc(sizeof(bool) * (PRIME_LIMIT + 1));
    if (prime_sieve == NULL) return;      /* memory failure – shouldn't happen */

    for (size_t i = 0; i <= PRIME_LIMIT; ++i) prime_sieve[i] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!prime_sieve[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                prime_sieve[j] = true;

    /* allocate space for the prime list (6000 > π(PRIME_LIMIT)) */
    prime_list = (uint32_t *)malloc(sizeof(uint32_t) * 6000);
    if (prime_list == NULL) return;
    prime_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT && prime_cnt < 6000; ++i)
        if (!prime_sieve[i])
            prime_list[prime_cnt++] = (uint32_t)i;
}

/* ------------------------------------------------------------------ */
/*  Arithmetic derivative of a number n (A003415).                     */
static seq_t arithmetic_derivative(idx_t n)
{
    if (n < 2) return 0;

    seq_t deriv = 0;
    idx_t rem = n;
    init_primes();

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint32_t p = prime_list[i];
        if ((idx_t)p * (idx_t)p > rem) break;
        if (rem % p == 0)
        {
            idx_t e = 0;
            while (rem % p == 0)
            {
                rem /= p;
                ++e;
            }
            /* contribution: n * e / p */
            deriv += ((seq_t)n * e) / p;
        }
    }
    if (rem > 1)   /* remaining prime factor > sqrt(n) */
    {
        /* its exponent is 1 */
        deriv += ((seq_t)n * 1) / rem;
    }
    return deriv;
}

/* ------------------------------------------------------------------ */
/*  Primorial-base exponent function A276086(n).                      */
static seq_t primorial_exponent(idx_t n)
{
    if (n == 0) return 1;   /* empty product */

    init_primes();

    /* Build primorials P[0] = 1, P[1] = 2, P[2] = 6, ... */
    idx_t primorials[32];
    size_t pcnt = 0;
    primorials[pcnt++] = 1;          /* P[0] */
    for (size_t i = 0; i < prime_cnt && pcnt < 32; ++i)
    {
        idx_t last = primorials[pcnt - 1];
        if (last > n / prime_list[i]) break;
        primorials[pcnt++] = (idx_t)last * (idx_t)prime_list[i];
    }

    /* Compute digits of n in primorial base */
    idx_t rem = n;
    idx_t digits[32] = {0};

    for (size_t k = pcnt; k > 0; )
    {
        --k;
        digits[k] = rem / primorials[k];
        rem -= digits[k] * primorials[k];
    }

    /* Product of prime[i] ^ digits[i] */
    seq_t result = 1;
    for (size_t i = 0; i < pcnt; ++i)
    {
        if (digits[i] == 0) continue;
        result *= pow_seq((seq_t)prime_list[i], (idx_t)digits[i]);
    }
    return result;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.                                             */
seq_t a(idx_t n)
{
    seq_t d = arithmetic_derivative(n);
    seq_t f = primorial_exponent(n);
    seq_t g = gcd(d, f);
    return arithmetic_derivative((idx_t)g);
}
