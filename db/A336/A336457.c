/*
 * A336457  a(n) = A065330(sigma(n)), where A065330 is fully multiplicative
 *          with a(2) = a(3) = 1, and a(p) = p for primes p > 3.
 * https://oeis.org/A336457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A065330( sigma(n) ) where A065330 is fully multiplicative
 *            with a(2)=a(3)=1, a(p)=p for primes p>3 i.e. a065330(m) =
 *            product of all prime powers of m with prime > 3. Thus a(n)
 *            removes the primes 2 and 3 from the prime‐factorisation of
 *            sigma(n)
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

static seq_t *a_cache = NULL;          /* 1‑based cache of a(n)   */
static idx_t a_cache_size = 0;         /* allocated size */

static uint64_t *prime_list = NULL;    /* primes up to the limit  */
static size_t prime_cnt = 0;           /* number of primes       */

/* Ensure cache has room for indices 0..n */
static void ensure_cache(idx_t n)
{
    if (a_cache_size <= n) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        /* initialise new entries to 0 (unset) */
        for (size_t i = a_cache_size; i < new_size; ++i) tmp[i] = 0;
        a_cache = tmp;
        a_cache_size = new_size;
    }
}

/* Build a list of primes once, up to a fixed limit */
static void init_prime_list(void)
{
    if (prime_cnt == 0) {
        /* 1 000 000 is ample for n up to ~10⁴ (sigma < 10⁵) */
        build_primes_upto(1000000, &prime_list, &prime_cnt);
    }
}

/* Compute sigma(n) for 64‑bit n */
static seq_t sigma_u64(uint64_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        seq_t term = 1;            /* sum of p^0 .. p^e */
        seq_t power = 1;
        for (int k = 1; k <= (int)ex[i]; ++k) {
            power *= p;
            term += power;
        }
        result *= term;
    }
    return result;
}

/* Compute A065330(x) for an arbitrary‑precision integer x.
   Multiply each prime power of x with prime > 3. */
static seq_t a065330(seq_t x)
{
    if (x <= 1) return 1;          /* sigma(1)=1 → answer 1 */

    init_prime_list();

    seq_t rad = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        seq_t p_seq = (seq_t)p;

        /* stop if p^2 > x */
        if (p_seq * p_seq > x) break;

        if (x % p == 0) {
            if (p > 3) {
                while (x % p == 0) {
                    rad *= p_seq;
                    x /= p_seq;
                }
            } else {
                while (x % p == 0) x /= p_seq;
            }
        }
    }
    /* Remaining factor (if any) is prime > sqrt(original) */
    if (x > 1) {
        /* x may be > 2 or 3; ignore them */
        if (x % 2 != 0 && x % 3 != 0) rad *= x;
    }
    return rad;
}

/* Main function: a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n=0 */
    ensure_cache(n);
    if (a_cache[n] != 0) return a_cache[n];

    uint64_t n64 = (uint64_t)n;
    seq_t sigma_val = sigma_u64(n64);
    seq_t result = a065330(sigma_val);
    a_cache[n] = result;
    return result;
}
