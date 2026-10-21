/*
 * A287919  Square array T(0,n) = prime(n) and T(m+1,n) = T(m,n) + T(m,n+1), m
 *          >= 0, n >= 1, read by falling antidiagonals.
 * https://oeis.org/A287919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) reads the square array defined by T(0,k) = prime(k)
 *            (prime(0)=2) T(m+1,k) = T(m,k) + T(m,k+1) for m,k ≥ 0 The terms
 *            are listed along falling antidiagonals: a(0)=T(0,0),
 *            a(1)=T(0,1), a(2)=T(1,0), a(3)=T(0,2),
 * Range:     u8 n <= 25, u16 n <= 89, u32 n <= 404, u64 n <= 1768,
 *            u128 n <= 7379, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation builds the list of primes on demand.  It
    works for any idx_t and seq_t (including arbitrary‑precision
    integer classes).  No external headers beyond the standard C
    headers are required.  The code compiles under C11 and C++17. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* static prime list, built lazily */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t     prime_count = 0;    /* number of primes stored   */
static size_t     prime_cap   = 0;    /* allocated capacity        */

/* Extend prime_list so that it contains primes up to index 'needed_index'
   (inclusive).  The array grows exponentially as needed. */
static void prime_extend_to(size_t needed_index)
{
    if (needed_index < prime_count)
        return;

    /* grow capacity */
    if (prime_cap <= needed_index) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 8;
        while (new_cap <= needed_index)
            new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) {
            /* unrecoverable – terminate */
            exit(1);
        }
        prime_list = tmp;
        prime_cap  = new_cap;
    }

    /* generate primes sequentially */
    uint64_t candidate = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;
    for (; prime_count <= needed_index; ++candidate) {
        bool is_prime = true;
        for (size_t j = 0; j < prime_count; ++j) {
            uint64_t p = prime_list[j];
            if (p * p > candidate)   /* p*p fits in 64 bits for our limits */
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_list[prime_count++] = candidate;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find diagonal index k and offsets (m,ncol) for given linear index n */
    idx_t k   = 0;
    idx_t rem = n;
    while (rem >= (k + 1)) {
        rem -= (k + 1);
        ++k;
    }
    idx_t m    = rem;            /* row index in the array    */
    idx_t ncol = k - rem;        /* column index in the array */

    /* Ensure we have all required primes */
    prime_extend_to((size_t)(ncol + m));

    /* Compute T(m,ncol) = sum_{i=0}^{m} binom(m,i) * prime[ncol+i] */
    seq_t sum   = 0;
    seq_t binom = 1;                 /* C(m,0) */
    for (idx_t i = 0; i <= m; ++i) {
        seq_t p    = (seq_t)prime_list[(size_t)(ncol + i)];
        sum += binom * p;
        if (i < m) {
            binom = binom * (seq_t)(m - i) / (seq_t)(i + 1);
        }
    }

    return sum;
}
