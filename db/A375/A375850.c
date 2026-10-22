/*
 * A375850  The maximum even exponent in the prime factorization of n!, or 0
 *          if no such exponent exists.
 * https://oeis.org/A375850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the maximum even exponent among the prime factors of n!;
 *            if all prime exponents in n! are odd, a(n) = 0
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Static storage for the list of primes found so far. */
static uint64_t *prime_list = NULL;   /* array of primes up to the last limit */
static size_t   prime_cnt  = 0;        /* number of primes stored */
static size_t   prime_cap  = 0;        /* allocated capacity of prime_list */
static idx_t    prime_limit = 0;       /* highest n for which primes were built */

/* Build or extend the prime list up to at least n. */
static void build_primes_upto(idx_t n)
{
    if (n <= prime_limit)
        return;               /* already have enough primes */

    size_t sz = (size_t)(n + 1);
    /* Allocate a bool array for the sieve (no implicit void* conversion in C++). */
    bool *is_comp = (bool *)calloc(sz, sizeof(bool));
    if (!is_comp)
        return;               /* allocation failure – return, caller will get 0 */

    /* Ensure prime_list capacity can hold all primes up to n. */
    size_t approx_primes = sz / 10 + 16;   /* rough upper bound, safe for n <= 1e6 */
    if (prime_cap < approx_primes) {
        uint64_t *newp = (uint64_t *)realloc(prime_list, approx_primes * sizeof(uint64_t));
        if (!newp) {
            free(is_comp);
            return;           /* allocation failure – return */
        }
        prime_list = newp;
        prime_cap  = approx_primes;
    }
    /* Reset prime count for new sieve */
    prime_cnt = 0;

    /* Classic Sieve of Eratosthenes. */
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[i]) {
            prime_list[prime_cnt++] = (uint64_t)i;
            if (prime_cnt == prime_cap) {
                /* Double capacity if needed. */
                size_t new_cap = prime_cap * 2;
                uint64_t *newp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!newp) {
                    free(is_comp);
                    return;
                }
                prime_list = newp;
                prime_cap  = new_cap;
            }
            if ((idx_t)i * i <= n) {
                for (idx_t j = (idx_t)i * i; j <= n; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    prime_limit = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure we have all primes up to n. */
    build_primes_upto(n);

    seq_t max_even = 0;                 /* the answer to return */
    unsigned long long n64 = n;         /* use 64‑bit arithmetic for exponents */

    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        unsigned long long p = prime_list[idx];
        unsigned long long e = 0;      /* exponent of prime p in n! */

        unsigned long long power = p;
        while (power <= n64) {
            e += n64 / power;
            if (power > n64 / p)       /* avoid overflow */
                break;
            power *= p;
        }

        if ((e & 1ULL) == 0) {                 /* even exponent */
            seq_t ev = (seq_t)e;
            if (ev > max_even)
                max_even = ev;
        }
    }

    return max_even;
}
