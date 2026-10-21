/*
 * A276421  Number of palindromic compositions of n into nonprime numbers.
 * https://oeis.org/A276421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is nonprime (including 1), 0 otherwise +
 *            Σ_{s=1}^{⌊n/2⌋} [s is nonprime] * a(n-2s) (a(0)=1)
 * Range:     u8 n <= 27, u16 n <= 53, u32 n <= 105, u64 n <= 211,
 *            u128 n <= 419, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence derived from
   G(z) = (1+F(z))/(1-F(z²)), where F(z)=∑_{m∈S} z^m and
   S = {1} ∪ {composite integers}. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *A = NULL;          /* array of a(k) values */
    static idx_t A_cap = 0;          /* number of elements computed so far */

    static unsigned char *isPrime = NULL; /* 1 if prime, 0 otherwise */
    static idx_t prime_limit = 0;         /* maximum n for which isPrime is valid */

    /* Ensure prime table up to n */
    if (n > prime_limit) {
        idx_t newLimit = n;
        unsigned char *tmp = (unsigned char *)realloc(isPrime, (size_t)(newLimit + 1));
        if (!tmp) abort();
        isPrime = tmp;

        for (idx_t i = 0; i <= newLimit; ++i) isPrime[i] = 1;
        if (newLimit >= 0) isPrime[0] = 0;
        if (newLimit >= 1) isPrime[1] = 0;

        for (idx_t i = 2; i * i <= newLimit; ++i) {
            if (isPrime[i]) {
                for (idx_t j = i * i; j <= newLimit; j += i)
                    isPrime[j] = 0;
            }
        }
        prime_limit = newLimit;
    }

    /* Ensure a(k) for k=0..n are computed */
    if (n + 1 > A_cap) {
        size_t newCap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(A, newCap * sizeof(seq_t));
        if (!tmp) abort();
        A = tmp;

        for (idx_t i = A_cap; i <= n; ++i) {
            if (i == 0) {            /* a(0) = 1 */
                A[0] = 1;
                continue;
            }

            seq_t sum = 0;
            idx_t half = i / 2;
            for (idx_t s = 1; s <= half; ++s) {
                if (!isPrime[s]) { /* s is nonprime */
                    sum += A[i - 2 * s];
                }
            }
            if (!isPrime[i]) sum += 1; /* add 1 if i itself is nonprime */
            A[i] = sum;
        }
        A_cap = n + 1;
    }

    return A[n];
}
