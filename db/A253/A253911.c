/*
 * A253911  Concatenation of n-th nonprime and n-th prime.
 * https://oeis.org/A253911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n)=concat(A018252(n),A000040(n))
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 9592, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, is_prime_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = concatenate the n-th nonprime (counting 1) with the n-th prime. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/pow10.h"          /* static seq_t pow10(idx_t e); */
#include "../utils/digit_len.h"      /* int digit_len(seq_t x); */
#include "../utils/is_prime_small.h" /* bool is_prime_small(seq_t n); */

static uint32_t *primes      = NULL;   /* first primes   */
static uint32_t *nonprimes   = NULL;   /* first nonprimes */
static idx_t    max_n  = 0;

/* Ensure we have at least `need` primes and `need` nonprimes. */
static void precompute(idx_t need)
{
    if (need <= max_n)
        return;

    /* Rough upper bound for the n-th prime: (~ n (log n + log log n)). */
    idx_t limit = (need * 20 + 100);     /* sufficient for n ≤ 10000 */

    /* Allocate and initialize sieve table. */
    bool *is_prime = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime) return;               /* out of memory – give up */

    for (idx_t i = 2; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    /* Allocate new storage for primes/nonprimes. */
    uint32_t *new_primes = (uint32_t *)malloc((size_t)need * sizeof(uint32_t));
    uint32_t *new_nonpr  = (uint32_t *)malloc((size_t)need * sizeof(uint32_t));
    if (!new_primes || !new_nonpr) {
        free(is_prime);
        free(new_primes);
        free(new_nonpr);
        return;
    }

    /* Collect primes. */
    size_t pc = 0;
    for (idx_t i = 2; pc < need && i <= limit; ++i)
        if (is_prime[i])
            new_primes[pc++] = (uint32_t)i;

    /* Collect nonprimes (1 is counted as nonprime). */
    size_t nc = 0;
    for (idx_t i = 1; nc < need && i <= limit; ++i)
        if (!is_prime[i])
            new_nonpr[nc++] = (uint32_t)i;

    /* If sieve didn't reach enough nonprimes, extend by primality test. */
    if (nc < need) {
        idx_t i = limit + 1;
        while (nc < need) {
            if (!is_prime_small((seq_t)i))
                new_nonpr[nc++] = (uint32_t)i;
            ++i;
        }
    }

    /* Replace old arrays. */
    free(primes);
    free(nonprimes);
    free(is_prime);

    primes      = new_primes;
    nonprimes   = new_nonpr;
    max_n       = need;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n = 0 */

    precompute(n);

    seq_t prime_val    = (seq_t)primes[n - 1];
    seq_t nonprime_val = (seq_t)nonprimes[n - 1];

    int d = digit_len(prime_val);                /* digits of prime */
    seq_t pow10_val = pow10((idx_t)d);            /* 10^d as seq_t */

    return nonprime_val * pow10_val + prime_val;
}
