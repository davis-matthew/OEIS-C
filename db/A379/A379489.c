/*
 * A379489  a(n) = gcd(n,A003961(n))*gcd(sigma(n),A276086(n)) -
 *          gcd(n,A276086(n))*gcd(sigma(n),A003961(n)), where A003961 is fully
 *          multiplicative with a(prime(i)) = prime(i+1), and A276086 is the
 *          primorial base exp-function.
 * https://oeis.org/A379489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = gcd(n, A003961(n))*gcd(sigma(n),A276086(n)) - gcd(n,
 *            A276086(n))*gcd(sigma(n),A003961(n))
 * Range:     i8 n <= 14, i16 n <= 57, i32 n <= 1544, i64 n <= 9804,
 *            i128 n <= 9804, big any n
 * Helpers:   factor_u64, gcd, next_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helpers defined in the build environment */
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/sigma_of.h"

#define MAX_PRIME_LIMIT 200000   /* enough for n up to 10^4 */
#define MAX_PRIMES      20000
#define MAX_PRIMORIALS  25

/* Static storage for primes and primorials */
static bool initialized = false;
static uint64_t primes[MAX_PRIMES];
static size_t prime_cnt = 0;
static uint64_t primorials[MAX_PRIMORIALS];
static size_t primorial_cnt = 0;

/* ------------------------------------------------------------ */
/* PRIME SIEVE:  generate primes up to MAX_PRIME_LIMIT          */
/* ------------------------------------------------------------ */
static void init_primes(void)
{
    if (initialized) return;
    bool *is_composite = (bool*)calloc(MAX_PRIME_LIMIT + 1, 1);
    if (!is_composite) exit(EXIT_FAILURE);

    for (uint64_t p = 2; p <= MAX_PRIME_LIMIT; ++p) {
        if (!is_composite[p]) {
            if (prime_cnt == MAX_PRIMES) break;   /* safety */
            primes[prime_cnt++] = p;
            if (p * p <= MAX_PRIME_LIMIT) {
                for (uint64_t q = p * p; q <= MAX_PRIME_LIMIT; q += p)
                    is_composite[q] = true;
            }
        }
    }
    free(is_composite);

    /* build initial primorials: P[0] = 1, P[i] = product of first i primes */
    primorials[0] = 1;
    primorial_cnt = 1;
    for (size_t i = 0; i + 1 < MAX_PRIMORIALS && i < prime_cnt; ++i) {
        primorials[primorial_cnt++] = primorials[primorial_cnt - 1] * primes[i];
    }
    initialized = true;
}

/* ------------------------------------------------------------ */
/* POWER: compute base^exp where exp fits in idx_t              */
/* ------------------------------------------------------------ */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ------------------------------------------------------------ */
/* A003961(n): fully multiplicative with a(p_i) = prime_{i+1}    */
/* ------------------------------------------------------------ */
static seq_t compute_A003961(idx_t n)
{
    if (n == 1) return 1;

    uint64_t rem = n;
    uint64_t pf[64], ex[64];
    size_t cnt = 0;
    factor_u64(rem, pf, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pf[i];
        uint64_t e = ex[i];
        uint64_t np = next_prime(p);              /* next prime after p  */
        seq_t powp = pow_seq((seq_t)np, (idx_t)e);
        result *= powp;
    }
    return result;
}

/* ------------------------------------------------------------ */
/* A276086(n): primorial base exp-function                      */
/* ------------------------------------------------------------ */
static seq_t compute_A276086(idx_t n)
{
    /* Prepare primorials up to > n if needed */
    while (primorial_cnt < MAX_PRIMORIALS &&
           primorials[primorial_cnt - 1] <= n) {
        if (primorial_cnt - 1 >= prime_cnt) break; /* no more primes available */
        primorials[primorial_cnt++] =
            primorials[primorial_cnt - 1] * primes[primorial_cnt - 1];
    }

    /* Find the largest index i with primorials[i] <= n */
    size_t i = 0;
    for (size_t j = primorial_cnt; j; --j) {
        if (primorials[j - 1] <= n) { i = j - 1; break; }
    }

    /* Compute digits of n in primorial base and then product of primes^digit */
    seq_t result = 1;
    idx_t rem = n;
    for (size_t k = i + 1; k-- > 0;) {
        idx_t digit = rem / primorials[k];
        rem %= primorials[k];
        if (digit) {
            seq_t base = (seq_t)primes[k];
            seq_t powval = pow_seq(base, digit);
            result *= powval;
        }
    }
    return result;
}

/* ------------------------------------------------------------ */
/* Main sequence function                                          */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) init_primes();

    seq_t nseq = (seq_t)n;

    seq_t a003961 = compute_A003961(n);
    seq_t sigma  = sigma_of(n);
    seq_t a276086 = compute_A276086(n);

    seq_t g1 = gcd(nseq, a003961);
    seq_t g2 = gcd(sigma, a276086);
    seq_t g3 = gcd(nseq, a276086);
    seq_t g4 = gcd(sigma, a003961);

    return g1 * g2 - g3 * g4;
}
