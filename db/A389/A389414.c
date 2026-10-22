/*
 * A389414  Numbers k whose (not necessarily distinct) prime factors can be
 *          side lengths of a simple polygon.
 * https://oeis.org/A389414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 4277, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A389414:
 * a(n) is the n‑th integer k ≥ 2 whose multiset of prime factors
 * (counted with multiplicity) satisfies 2·(largest prime factor)
 * < sum of all prime factors, i.e. the prime factors can be the side
 * lengths of a simple polygon. This implementation performs a linear
 * search up to k and factors each candidate using a cached list of
 * primes. No external library functions are required. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*----------------------------------------------------------------------*
 * Prime list management (simple incremental sieve).
 *----------------------------------------------------------------------*/
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t   prime_cnt  = 0;       /* number of primes stored   */
static size_t   prime_cap  = 0;       /* current allocation size   */
static uint64_t prime_limit = 1;      /* largest number checked   */

/* Ensure prime_list contains all primes ≤ limit. */
static void
ensure_primes_upto(uint64_t limit)
{
    if (limit <= prime_limit) return;

    /* Grow the array if needed. */
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                            new_cap * sizeof(uint64_t));
        if (!tmp) abort();                /* allocation failed */
        prime_list = tmp;
        prime_cap = new_cap;
    }

    /* Incrementally find primes from prime_limit+1 to limit. */
    for (uint64_t n = prime_limit + 1; n <= limit; ++n) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p > n / p) break;
            if (n % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            prime_list[prime_cnt++] = n;
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_list = tmp;
                prime_cap = new_cap;
            }
        }
    }
    prime_limit = limit;
}

/*----------------------------------------------------------------------*
 * Main sequence function.
 *----------------------------------------------------------------------*/
seq_t
a(idx_t n)
{
    if (n == 0) {          /* offset is 1; avoid undefined behaviour */
        return (seq_t)0;
    }

    /* Ensure we have some primes ready. */
    if (prime_cnt == 0) {
        ensure_primes_upto(2);
    }

    size_t count  = 0;          /* how many terms found so far */
    uint64_t k    = 2;          /* candidate integer to test */

    while (1) {
        /* Grow prime list to cover sqrt(k). */
        uint64_t sqrt_k = (uint64_t)(0.5 + (double)k * 0.5);  /* rough */
        /* if needed, add primes up to k to be safe */
        ensure_primes_upto(k);

        uint64_t m = k;
        seq_t   sum  = 0;
        seq_t   largest = 0;

        /* Factor using primes list. */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p > m / p) break;
            if (m % p == 0) {
                do {
                    m /= p;
                    seq_t sp = (seq_t)p;
                    sum += sp;
                    if (sp > largest) largest = sp;
                } while (m % p == 0);
            }
        }
        if (m > 1) {            /* remaining prime factor > sqrt(k) */
            seq_t sp = (seq_t)m;
            sum += sp;
            if (sp > largest) largest = sp;
        }

        /* Polygon inequality: 2*largest < sum. */
        if ((largest << 1) < sum) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
}
