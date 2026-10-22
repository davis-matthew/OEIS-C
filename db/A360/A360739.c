/*
 * A360739  Semiprimes of the form k^2 + 2.
 * https://oeis.org/A360739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A242330(n)^2 + 2 (where A242330 lists k with k²+2
 *            semiprime) Computed by scanning k and keeping those k such that
 *            k²+2 is a semiprime
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"    /* build_primes_upto */
#include "../utils/floor_sqrt.h"          /* floor_sqrt */

/* static cache of computed sequence values */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* static list of primes for trial division */
static uint64_t *prime_arr = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* make sure we have primes up to 'limit' (inclusive) in prime_arr */
static void ensure_primes_upto(idx_t limit)
{
    if (prime_limit >= limit)
        return;
    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);
    free(prime_arr);
    prime_arr = new_primes;
    prime_cnt = new_cnt;
    prime_limit = limit;
}

/* expand the sequence cache if necessary */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* test whether x is a semiprime (exactly two prime factors counting multiplicity) */
static bool is_semiprime(seq_t x)
{
    if (x < 6)              /* minimal semiprime is 6 = 2*3 */
        return false;
    seq_t m = x;
    unsigned factors = 0;

    /* Ensure we have enough primes for trial division */
    seq_t root = floor_sqrt(m);          /* floor sqrt of m (seq_t) */
    ensure_primes_upto((idx_t)root + 1);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        seq_t pp = (seq_t)p * (seq_t)p;
        if (pp > m)
            break;
        while (m % (seq_t)p == 0) {
            ++factors;
            m /= (seq_t)p;
            if (factors > 2)
                return false;
        }
    }
    if (m > 1)                       /* remaining prime factor */
        ++factors;
    return factors == 2;
}

/* Return the n‑th term of A360739 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n<1 but avoid out‑of‑bounds */

    static idx_t last_k = 0;   /* smallest k not yet considered */

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        ++last_k;
        seq_t val = (seq_t)last_k * (seq_t)last_k + (seq_t)2;
        if (is_semiprime(val)) {
            ensure_seq_cap(seq_len + 1);
            seq_cache[seq_len++] = val;
        }
    }
    return seq_cache[n - 1];
}
