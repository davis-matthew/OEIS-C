/*
 * A331999  a(n) is the product of n, the n-th prime and the n-th composite
 *          number.
 * https://oeis.org/A331999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*prime(n)*composite(n)
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 777, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

static idx_t *primes           = NULL;       /* stored primes [0 .. prime_cnt-1] */
static size_t prime_cap        = 0;          /* allocated capacity for primes */
static size_t prime_cnt        = 0;          /* number of primes stored */

static idx_t *composites      = NULL;       /* stored composites [0 .. comp_cnt-1] */
static size_t comp_cap          = 0;          /* allocated capacity for composites */
static size_t comp_cnt          = 0;          /* number of composites stored */

static idx_t prime_test_start  = 2;          /* next candidate to test for prime */
static idx_t comp_test_start   = 4;          /* next candidate to test for composite */

/* Grow the primes array when needed. */
static void grow_primes(void)
{
    if (prime_cap == 0)
        prime_cap = 16;
    else
        prime_cap <<= 1;      /* double */
    primes = (idx_t *)realloc(primes, prime_cap * sizeof(idx_t));
    if (!primes) abort();     /* allocation failure */
}

/* Grow the composites array when needed. */
static void grow_composites(void)
{
    if (comp_cap == 0)
        comp_cap = 16;
    else
        comp_cap <<= 1;
    composites = (idx_t *)realloc(composites, comp_cap * sizeof(idx_t));
    if (!composites) abort();
}

/* Make sure we have at least `n` primes and `n` composites stored. */
static void ensure_n(idx_t n)
{
    /* Generate primes */
    while (prime_cnt < n) {
        while (!is_prime(prime_test_start))
            ++prime_test_start;
        if (prime_cnt == prime_cap)
            grow_primes();
        primes[prime_cnt++] = prime_test_start++;
    }

    /* Generate composites */
    while (comp_cnt < n) {
        while (!(comp_test_start > 1 && !is_prime(comp_test_start)))
            ++comp_test_start;
        if (comp_cnt == comp_cap)
            grow_composites();
        composites[comp_cnt++] = comp_test_start++;
    }
}

/* Sequence term a(n). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */
    ensure_n(n);
    seq_t result = (seq_t)n;
    result *= (seq_t)primes[n - 1];
    result *= (seq_t)composites[n - 1];
    return result;
}
