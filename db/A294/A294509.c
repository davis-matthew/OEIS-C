/*
 * A294509  a(n) is the least value of pi(n*m) - pi(n)*pi(m) for any positive
 *          m <= n.
 * https://oeis.org/A294509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 48, i16 n <= 1262, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A294509:  a(n) = min_{1≤m≤n} ( pi(n·m) – pi(n)·pi(m) )
 *  The code builds a sieve of Eratosthenes up to n² and stores the list of primes.
 *  Prime counts are obtained by binary search on the prime list.  The algorithm
 *  is O(n) per call after the sieve has been constructed; the sieve itself
 *  is computed only when a larger n is requested. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- static storage for the sieve and primes ---------- */
static unsigned char *comp_bits_arr   = NULL;   /* bitset of odd composites  */
static size_t        comp_bits_len   = 0;       /* length in bits           */
static idx_t         sieve_limit     = 0;       /* maximum n² handled so far */
static uint32_t *    primes          = NULL;   /* list of primes ≤ limit   */
static size_t        primes_count    = 0;       /* number of primes          */

/* ---------- helpers for the bitset of odd composites ---------- */
#define COMP_GET(idx)   (comp_bits_arr[(idx)>>3] & (1u << ((idx)&7)))
#define COMP_SET(idx)   (comp_bits_arr[(idx)>>3] |= (1u << ((idx)&7)))

/* ---------- extend the sieve to cover values up to 'limit' ---------- */
static void extend_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;          /* already sufficient */

    /* free previous data */
    free(comp_bits_arr);
    free(primes);

    sieve_limit = limit;
    comp_bits_len = ((limit >> 1u) + 7u) >> 3u;     /* one bit per odd number */
    comp_bits_arr = (unsigned char *)calloc(comp_bits_len, 1);
    if (!comp_bits_arr) abort();                  /* allocation failed */

    /* Sieve of Eratosthenes for odd numbers only */
    for (idx_t i = 3; i <= limit; i += 2) {
        if (!COMP_GET(i >> 1u)) {                 /* i is prime */
            if (i * i <= limit) {                /* avoid overflow */
                for (idx_t j = i * i; j <= limit; j += i << 1u)
                    COMP_SET(j >> 1u);
            }
        }
    }

    /* Build the prime list */
    size_t cap = 1024;
    primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (!primes) abort();

    primes_count = 0;
    if (limit >= 2) {
        primes[primes_count++] = 2u;
        for (idx_t v = 3; v <= limit; v += 2) {
            if (!COMP_GET(v >> 1u)) {
                if (primes_count == cap) {
                    cap <<= 1;
                    primes = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
                    if (!primes) abort();
                }
                primes[primes_count++] = (uint32_t)v;
            }
        }
    }
}

/* ---------- prime counting function using binary search ---------- */
static inline seq_t pi_of(idx_t x)
{
    if (x < 2) return 0;
    size_t lo = 0, hi = primes_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((idx_t)primes[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)lo;
}

/* ---------- the required sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* defined for positive indices only */

    idx_t max_product = n * n;                 /* largest argument to pi */
    extend_sieve(max_product);

    seq_t pi_n = pi_of(n);
    seq_t min_val = pi_n;                     /* m = 1 gives pi(n) */

    for (idx_t m = 2; m <= n; ++m) {
        seq_t pi_m         = pi_of(m);
        seq_t pi_nm        = pi_of(n * m);
        seq_t diff         = pi_nm - pi_n * pi_m;
        if (diff < min_val) min_val = diff;
    }
    return min_val;
}
