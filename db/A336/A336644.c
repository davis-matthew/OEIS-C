/*
 * A336644  a(n) = (n-rad(n)) / core(n), where rad(n) and core(n) give the
 *          squarefree kernel and squarefree part of n, respectively.
 * https://oeis.org/A336644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - rad(n)) / core(n) where rad(n) = product of distinct
 *            primes dividing n core(n) = product of primes dividing n to an
 *            odd exponent
 * Range:     u8 n <= 287, u16 n <= 9761, u32 n <= 9761, u64 n <= 9761,
 *            u128 n <= 9761, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *spf_arr = NULL;  /* smallest prime factor for each integer */
static size_t spf_len = 0;        /* current size of spf_arr (indices 0..spf_len-1) */

static void ensure_spf(size_t n) {
    if (n < spf_len) return;                 /* already computed enough */
    /* allocate new array with size n+1 */
    uint64_t *new_arr = (uint64_t *)realloc(spf_arr, (n + 1) * sizeof(uint64_t));
    if (!new_arr) abort();                   /* allocation failure */
    spf_arr = new_arr;
    /* initialize with 0; work on all indices up to n */
    for (size_t i = 0; i <= n; ++i) spf_arr[i] = 0;
    /* Sieve of Eratosthenes for smallest prime factor */
    for (size_t i = 2; i <= n; ++i) {
        if (spf_arr[i] == 0) {               /* i is prime */
            spf_arr[i] = (uint64_t)i;
            if (i <= n / i) {                /* avoid overflow on i*i */
                for (size_t j = i * i; j <= n; j += i) {
                    if (spf_arr[j] == 0) spf_arr[j] = (uint64_t)i;
                }
            }
        }
    }
    spf_len = n + 1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset starts at 1, but guard just in case */

    /* Ensure we have spf information up to n */
    ensure_spf((size_t)n);

    uint64_t nn = (uint64_t)n;
    uint64_t rad = 1;   /* radical of n */
    uint64_t core = 1;  /* core: product of primes with odd exponent */

    uint64_t tmp = nn;
    while (tmp > 1) {
        uint64_t p = spf_arr[tmp];
        size_t exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++exp;
        }
        rad *= p;
        if ((exp & 1) != 0) core *= p;
    }

    /* Compute (n - rad) / core */
    seq_t numer = (seq_t)nn - (seq_t)rad;   /* numerator fits in seq_t as n >= rad */
    /* core divides the numerator exactly */
    seq_t result = numer / (seq_t)core;
    return result;
}
