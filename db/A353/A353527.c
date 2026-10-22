/*
 * A353527  The smallest prime not dividing 2*n, reduced modulo 4.
 * https://oeis.org/A353527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9438, u16 n <= 9438, u32 n <= 9438, u64 n <= 9438,
 *            u128 n <= 9438, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353527: the smallest prime not dividing 2*n, reduced modulo 4.
   Formula (selected): a(n) = A353526(2*n) = A010873(A053669(2*n)).
   Implementation: sieve once, then for each n test primes in ascending order until
   a prime p that does not divide 2*n is found; return p mod 4. */

/* Required headers */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static data for prime sieve */
static uint32_t *g_primes = NULL;   /* array of primes */
static size_t   g_prime_cnt = 0;    /* number of primes in g_primes */

/* Build a small list of primes (sufficient for n up to 10000) */
static void init_primes(void)
{
    if (g_prime_cnt != 0)           /* already initialized */
        return;

    /* Upper bound for primes needed. For n=10000, 2*n=20000, primes up to 20000 are enough. */
    const size_t LIMIT = 20000;
    bool *comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!comp) exit(1);

    /* Estimate capacity of prime array: number of primes <= LIMIT is < LIMIT / log(LIMIT) */
    size_t cap = LIMIT / 10;
    g_primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!g_primes) { free(comp); exit(1); }

    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!comp[i]) {
            if (cnt >= cap) {
                cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(g_primes, cap * sizeof(uint32_t));
                if (!tmp) { free(comp); free(g_primes); exit(1); }
                g_primes = tmp;
            }
            g_primes[cnt++] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    comp[j] = true;
            }
        }
    }
    g_prime_cnt = cnt;
    free(comp);
}

/* Sequence entry function */
seq_t a(idx_t n)
{
    /* Offset is 1; for n < 1 return 0 (unused in tests). */
    if (n < 1)
        return (seq_t)0;

    init_primes();          /* ensure primes are ready */

    /* Test each odd prime starting from 3 until one does not divide 2*n. */
    for (size_t i = 1; i < g_prime_cnt; ++i) {   /* g_primes[0] == 2 */
        uint32_t p = g_primes[i];
        /* Compute 2*n modulo p; use modulo operation to avoid overflow. */
        uint32_t rem = (uint32_t)(n % p);
        if ((rem * 2u) % p != 0u) {
            return (seq_t)(p % 4u);
        }
    }

    /* Fallback: if we ran out of primes (unlikely for n <= 10000),
       return the smallest prime > 2, which is 3. */
    return (seq_t)(3u % 4u);
}
