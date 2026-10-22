/*
 * A351465  Let f be multiplicative with f(prime(k)^e) = k + e*i for any k, e
 *          > 0 (where i denotes the imaginary unit); a(n) is the imaginary
 *          part of f(n). See A351464 for the real part.
 * https://oeis.org/A351465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A351465 – imaginary part of multiplicative function f
 *  where f(p_k^e) = k + e·i (p_k is the k‑th prime).  a(1)=0.
 *  The function computes a(n) for any n ≥ 1 and memoises a prime table
 *  up to the largest n seen so far.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

static uint64_t *prime_list = NULL;   /* primes 2,3,5,... (0‑based) */
static size_t   prime_count = 0;
static idx_t    prime_limit = 0;      /* maximum n for which primes are stored */

/* Ensure prime_list contains all primes ≤ limit. */
static void
ensure_prime_list(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    if (prime_list != NULL) {
        free(prime_list);
        prime_list = NULL;
    }

    build_primes(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Binary search for 1‑based index of prime p (0 if not found). */
static size_t
prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < prime_count && prime_list[lo] == p) ? (lo + 1) : 0;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n <= 1u)            /* a(1) = 0 */
        return 0;

    /* Build prime table up to n for factorisation and indexing. */
    ensure_prime_list(n);
    if (prime_list == NULL)   /* build_primes failed – return 0 as safe value */
        return 0;

    uint64_t m = (uint64_t)n;
    seq_t re = 1;            /* real part, intermediate */
    seq_t im = 0;            /* imaginary part, intermediate */

    /* Factorise m using trial division with the prime table. */
    for (size_t i = 0; i < prime_count && m > 1; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > m)      /* remaining factor is prime or 1 */
            break;

        if (m % p != 0)
            continue;

        /* Count exponent e of prime p in m. */
        size_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* k is the 1‑based index of prime p. */
        size_t k = i + 1;

        /* Multiply (re + im·i) by (k + e·i).  Use seq_t arithmetic. */
        seq_t new_re = re * (seq_t)k - im * (seq_t)e;
        seq_t new_im = re * (seq_t)e + im * (seq_t)k;
        re = new_re;
        im = new_im;
    }

    /* If a prime factor greater than sqrt(m) remains. */
    if (m > 1) {
        uint64_t p = m;
        size_t e = 1;                    /* exponent is 1 */
        size_t k = prime_index(p);       /* must exist because primes were built up to n */
        if (k == 0)
            return 0;                    /* safety, should never happen */

        seq_t new_re = re * (seq_t)k - im * (seq_t)e;
        seq_t new_im = re * (seq_t)e + im * (seq_t)k;
        re = new_re;
        im = new_im;
    }

    return (seq_t)im;
}
