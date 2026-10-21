/*
 * A280830  Number of partitions of n into two products-of-three-primes.
 * https://oeis.org/A280830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=2..⌊n/2⌋} A101605(i) * A101605(n-i) (where
 *            A101605(m) is the number of unordered products of three primes
 *            that equal m)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --------------------------------------------------------------
   Prime table helpers
   -------------------------------------------------------------- */
static idx_t      prime_max   = 0;          /* largest number sieved */
static uint64_t  *prime_head  = NULL;       /* array of primes up to prime_max */
static size_t     prime_count = 0;          /* number of primes stored */
static bool      *is_prime    = NULL;       /* boolean sieve array size prime_max+1 */

/* expand the prime table to at least `limit` */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_max) return;          /* already good */

    /* free previous tables */
    free(is_prime);
    free(prime_head);
    if (limit == 0) return;

    /* create sieve */
    size_t cap = (size_t)limit + 1;
    is_prime = (bool *) calloc(cap, sizeof(bool));
    if (!is_prime) abort();

    for (size_t i = 2; i <= (size_t)limit; ++i)
        is_prime[i] = true;

    for (size_t p = 2; p * p <= (size_t)limit; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= (size_t)limit; m += p)
                is_prime[m] = false;
        }
    }

    /* collect primes into a dynamic array */
    size_t est = limit / 10 + 10;               /* rough estimate of prime count */
    prime_head = (uint64_t *) malloc(est * sizeof(uint64_t));
    if (!prime_head) abort();
    prime_count = 0;

    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (is_prime[i]) {
            if (prime_count == est) {
                est *= 2;
                uint64_t *tmp = (uint64_t *) realloc(prime_head, est * sizeof(uint64_t));
                if (!tmp) abort();
                prime_head = tmp;
            }
            prime_head[prime_count++] = (uint64_t) i;
        }
    }

    prime_max = limit;
}

/* --------------------------------------------------------------
   Count unordered products of three primes for a single m
   -------------------------------------------------------------- */
static seq_t count_triplet_products(idx_t m)
{
    if (m < (idx_t)8)           /* 2*2*2 = 8 is the smallest product */
        return (seq_t)0;

    seq_t cnt = 0;

    /* iterate over triples p <= q <= r */
    for (size_t pi = 0; pi < prime_count; ++pi) {
        idx_t p = (idx_t) prime_head[pi];
        if (p * p * p > m) break;                /* cube bound */
        if (m % p != 0) continue;

        idx_t m2 = m / p;                         /* remaining product q*r */

        for (size_t qi = pi; qi < prime_count; ++qi) {
            idx_t q = (idx_t) prime_head[qi];
            if (q * q > m2) break;                /* sqrt bound on q */
            if (m2 % q != 0) continue;

            idx_t r = m2 / q;
            if (r < q) continue;                  /* enforce q <= r */

            /* r must be prime */
            if (r <= prime_max && is_prime[(size_t) r])
                ++cnt;
        }
    }

    return cnt;
}

/* --------------------------------------------------------------
   Cache for A101605 values
   -------------------------------------------------------------- */
static seq_t *A_array      = NULL;     /* cache of A101605 values */
static size_t A_allocated  = 0;        /* size of allocated array   */
static idx_t   A_highest   = 0;        /* highest n computed so far */

/* grow cache and compute missing entries up to `limit` */
static void ensure_A(idx_t limit)
{
    if (limit <= A_highest) return;                /* already done */

    if ((size_t)limit + 1 > A_allocated) {
        size_t new_size = (size_t) limit + 1;
        seq_t *tmp = (seq_t *) realloc(A_array, new_size * sizeof(seq_t));
        if (!tmp) abort();
        A_array = tmp;
        for (size_t i = A_allocated; i < new_size; ++i)
            A_array[i] = (seq_t)0;
        A_allocated = new_size;
    }

    for (idx_t i = A_highest + 1; i <= limit; ++i)
        A_array[i] = count_triplet_products(i);

    A_highest = limit;
}

/* --------------------------------------------------------------
   Main OEIS sequence function
   -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n=0 */

    /* prepare necessary data structures up to n */
    ensure_primes(n);
    ensure_A(n);

    seq_t sum = (seq_t)0;
    for (idx_t i = 2; i <= n / 2; ++i)
        sum += (A_array[i] * A_array[n - i]);

    return sum;
}
