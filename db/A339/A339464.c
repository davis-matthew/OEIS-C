/*
 * A339464  a(n) = (prime(n)-1) / gpf(prime(n)-1) where gpf(m) is the greatest
 *          prime factor of m, A006530.
 * https://oeis.org/A339464
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)-1) / gpf(prime(n)-1)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/gpf.h"

static uint64_t *prime_cache = NULL;
static size_t prime_cnt   = 0;
static idx_t    prime_limit = 0;

/* estimate an upper bound for the nth prime */
static idx_t estimate_limit(idx_t n)
{
    if (n < 6) return 15;                /* small values */
    double dn = (double)n;
    double est = dn * (log(dn) + log(log(dn))) + 10.0;
    return (idx_t)(est + 0.5);
}

/* generate all primes <= limit and store them in prime_cache */
static void generate_primes_upto(idx_t limit)
{
    free(prime_cache);
    prime_cache = NULL;
    prime_cnt = 0;

    /* boolean array for composite marks */
    size_t sz = (size_t)limit + 1;
    unsigned char *is_comp = (unsigned char*)calloc(sz, 1);
    if (!is_comp) return;

    /* dynamic array for collected primes */
    size_t cap = 1024;
    prime_cache = (uint64_t*)malloc(cap * sizeof(uint64_t));
    if (!prime_cache) {
        free(is_comp);
        return;
    }

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt == cap) {
                cap <<= 1;
                uint64_t *tmp = (uint64_t*)realloc(prime_cache, cap * sizeof(uint64_t));
                if (!tmp) break;          /* allocation failure → stop */
                prime_cache = tmp;
            }
            prime_cache[prime_cnt++] = (uint64_t)i;

            if ((uint64_t)i * (uint64_t)i <= (uint64_t)limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    prime_limit = limit;
}

/* ensure that prime_cache has at least n primes stored */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;
    idx_t new_limit = estimate_limit(n);
    generate_primes_upto(new_limit);
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;           /* offset not defined for n<2 */
    ensure_primes(n);

    uint64_t p = prime_cache[(size_t)(n - 1)];   /* prime(n) */
    uint64_t m = p - 1;
    uint64_t g = gpf(m);
    return (seq_t)(m / g);
}
