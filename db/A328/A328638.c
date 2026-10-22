/*
 * A328638  a(n) is the sum of m such that 2 <= m <= n and gpf(m) = gpf(n),
 *          where gpf(i) is the greatest prime factor of i (A006530), with
 *          a(1) = 1.
 * https://oeis.org/A328638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{m = 2..n, gpf(m)=gpf(n)} m, a(1)=1
 * Range:     u8 n <= 59, u16 n <= 1949, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t cached = 0;
    static uint64_t *gpf = NULL;        /* greatest prime factor of each number */
    static seq_t   *sumPerPrime = NULL; /* cumulative sum of numbers with a given gpf */

    /* base case */
    if (n == 1) return 1;

    /* need to rebuild data for larger n */
    if (n > cached) {
        /* free old data */
        free(gpf);
        free(sumPerPrime);

        /* allocate fresh arrays – indices 0..n inclusive */
        gpf        = (uint64_t*)calloc((size_t)(n + 1), sizeof(uint64_t));
        sumPerPrime = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));

        /* simple sieve for primes up to n */
        bool *isPrime = (bool*)malloc((size_t)(n + 1) * sizeof(bool));
        for (idx_t i = 0; i <= n; ++i) isPrime[i] = true;
        if (n >= 0) isPrime[0] = false;
        if (n >= 1) isPrime[1] = false;
        for (idx_t p = 2; p * p <= n; ++p) {
            if (isPrime[p]) {
                for (idx_t m = p * p; m <= n; m += p)
                    isPrime[m] = false;
            }
        }

        /* compute gpf – larger primes overwrite smaller ones */
        for (idx_t p = 2; p <= n; ++p) {
            if (isPrime[p]) {
                for (idx_t m = p; m <= n; m += p)
                    gpf[m] = p;
            }
        }

        /* build cumulative sums per prime */
        for (idx_t m = 2; m <= n; ++m) {
            uint64_t p = gpf[m];
            sumPerPrime[p] += (seq_t)m;
        }

        free(isPrime);
        cached = n;
    }

    /* answer for the requested n */
    return sumPerPrime[gpf[n]];
}
