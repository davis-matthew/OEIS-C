/*
 * A372007  a(n) = product of those prime(k) such that floor(n/prime(k)) is
 *          even.
 * https://oeis.org/A372007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of primes p ≤ n such that floor(n/p) is even
 * Range:     u8 n <= 23, u16 n <= 43, u32 n <= 81, u64 n <= 157,
 *            u128 n <= 283, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a dynamic prime table up to max(n) requested. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Static memory for the primes list. */
static uint32_t *primes = NULL;
static size_t   prime_cnt = 0;
static size_t   prime_cap = 0;
static idx_t    primes_up_to = 1;   /* the largest number up to which primes are known */

/* Ensure that all primes ≤ limit are in the primes array. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= primes_up_to)
        return;

    for (idx_t v = primes_up_to + 1; v <= limit; ++v) {
        if (v < 2)
            continue;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > v)
                break;
            if (v % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = (prime_cap ? (prime_cap << 1) : 16);
                uint32_t *tmp = (uint32_t *)realloc(primes, prime_cap * sizeof(uint32_t));
                if (!tmp)
                    exit(1);    /* cannot allocate */
                primes = tmp;
            }
            primes[prime_cnt++] = (uint32_t)v;
        }
    }
    primes_up_to = limit;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)1;   /* sequence defined for n ≥ 1, but be defensive */

    ensure_primes(n);

    seq_t res = (seq_t)1;
    seq_t N   = (seq_t)n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if (p > n)
            break;
        seq_t div = N / (seq_t)p;    /* floor division */
        if ((div & 1) == 0)          /* even floor(n/p) */
            res *= (seq_t)p;
    }

    return res;
}
