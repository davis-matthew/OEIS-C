/*
 * A296922  Primes p such that Legendre(-5,p) = 0 or 1.
 * https://oeis.org/A296922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 3240, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algo: returns the n-th prime p with Legendre(-5,p)=0 or 1.
   Such primes satisfy p ≡ 1,3,5,7,9 (mod 20). */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n<1 */

    /* Rough upper bound: primes of interest have density ≈1/2 of all primes. */
    idx_t limit = n * 25;            /* initial guess, >5 */
    if (limit < 100) limit = 100;    /* avoid tiny limits */

    for (;; limit <<= 1)             /* double until we have enough good primes */
    {
        size_t sz = (size_t)(limit + 1);
        bool *isp = (bool *)calloc(sz, sizeof(bool));
        if (!isp) return 0;          /* allocation failure */

        for (idx_t i = 2; i <= limit; ++i) isp[i] = true;
        for (idx_t p = 2; (idx_t)p * p <= limit; ++p)
            if (isp[p])
                for (idx_t m = p * p; m <= limit; m += p)
                    isp[m] = false;

        idx_t count = 0;
        seq_t result = 0;
        for (idx_t p = 2; p <= limit; ++p) {
            if (!isp[p]) continue;
            idx_t r = p % 20;
            if (r == 1 || r == 3 || r == 5 || r == 7 || r == 9) {
                ++count;
                if (count == n) {
                    result = (seq_t)p;
                    free(isp);
                    return result;
                }
            }
        }
        free(isp);
        /* not enough good primes found; double limit and retry */
    }
}
