/*
 * A378543  Sum of divisors d of n such that n/d has an odd number of prime
 *          factors (counted with multiplicity).
 * https://oeis.org/A378543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} d if parity_of_bigomega(n/d) = 1 (i.e. n/d has an
 *            odd total number of prime factors, counted with multiplicity).
 *            For n = 1..24 this gives
 *            0,1,1,2,1,5,1,5,3,7,1,11,1,9,8,10,1,16,1,15,10,13,1,25
 * Range:     u8 n <= 209, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 104 terms through n = 9757 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Parity of Ω(k) : returns true if the total number of prime factors
   (counted with multiplicity) of k is odd.  k is at most sizeof(idx_t). */
static inline bool parity_bigomega(idx_t k)
{
    int parity = 0;
    idx_t p;

    for (p = 2; p * p <= k; ++p)
        while (k % p == 0) {
            parity ^= 1;
            k /= p;
        }

    if (k > 1)
        parity ^= 1;           /* remaining prime factor */
    return parity;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Public sequence entry point.  Uses only integer arithmetic. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined outside the offset */

    seq_t sum = 0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;

            if (parity_bigomega(d2))
                sum += (seq_t)d1;
            if (d1 != d2 && parity_bigomega(d1))
                sum += (seq_t)d2;
        }
    }
    return sum;
}
