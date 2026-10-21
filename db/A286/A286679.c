/*
 * A286679  a(n) = (2*prime(n)^2 + 1)/3.
 * https://oeis.org/A286679
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*prime(n)^2 + 1)/3 where prime(n) is the n‑th prime
 *            (prime(1) = 2). The function works for all n ≥ 3. It lazily
 *            builds a prime cache using trial division; the cache is
 *            preserved across calls so that an arbitrary order of indices is
 *            supported. The implementation uses only integer arithmet...
 * Range:     u8 n <= 5, u16 n <= 64, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cached prime list – grows on demand.  prime_cache[i] == prime(i+1). */
static uint32_t *prime_cache = NULL;
static idx_t    prime_cnt   = 0;          /* number of primes cached */
static uint32_t candidate   = 2;          /* next integer to test for primality */

/* Ensure that prime_cache contains at least `target` primes.
   Generates primes by trial division against previously found primes. */
static void
ensure_prime_count(idx_t target)
{
    while (prime_cnt < target) {
        bool is_prime = true;
        for (idx_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = prime_cache[i];
            if ((uint64_t)p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /* grow the cache */
            uint32_t *tmp = (uint32_t *)realloc(prime_cache,
                                                (prime_cnt + 1) * sizeof(uint32_t));
            if (!tmp)
                exit(1); /* allocation failure */
            prime_cache = tmp;
            prime_cache[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* Main sequence function.  It satisfies external linkage and the
   required signature. */
seq_t
a(idx_t n)
{
    /* The offset of the sequence is 3; for n < 3 the behaviour is
       undefined, but the code still returns a value. */
    if (n == 0)
        return 0;

    /* Build enough primes to have prime(n). */
    ensure_prime_count(n);

    uint32_t p = prime_cache[n - 1];          /* prime(n)  (0‑based index) */

    /* Compute (2*prime(n)^2 + 1)/3 using seq_t arithmetic to avoid
       overflow for arbitrary‑precision builds. */
    seq_t prime_seq = (seq_t)p;
    seq_t val = prime_seq * prime_seq;   /* prime^2 */
    val *= (seq_t)2;                      /* 2*prime^2 */
    val += (seq_t)1;                      /* +1 */
    return val / (seq_t)3;                /* exact division by 3 */
}
