/*
 * A359309  Number of divisors of 6*n-5 of form 6*k+1.
 * https://oeis.org/A359309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of (6*n-5) such that d ≡ 1 (mod 6)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors (6*n-5) by trial division and counts the
   divisors that are ≡1 (mod 3).  The condition reduces to
   "d is odd" (always true for divisors of 6*n-5) and
   "d ≡ 1 (mod 3)".  For primes p ≡ 1 (mod 3) all exponents are allowed.
   For primes p ≡ 2 (mod 3) the parity of the exponents matters: the
   product of their contributions must have an even number of odd
   exponents.  The number of such divisors is computed by using the
   generating‑function trick:

       total = ∏(e_i+1)          (total number of divisors)
       diff  = ∏_{p≡2 (mod 3)}(E_i-O_i) * ∏_{p≡1 (mod 3)}(e_i+1)

   where for a prime p^e,
       E_i = e/2 + 1   (count of even exponents 0…e)
       O_i = (e+1)/2   (count of odd exponents 1…e)

   Then the desired count is (total + diff)/2.  This formula works for
   any size of seq_t and uses only integer arithmetic. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 6*n-5 fits in 64‑bit for the intended range (n ≤ 10⁴). */
    uint64_t N = (uint64_t)n * 6ULL - 5ULL;

    seq_t total = 1;   /* product of (e+1) over all prime powers */
    seq_t diff  = 1;   /* product needed to separate even/odd parity */

    uint64_t m = N;
    for (uint64_t p = 3ULL; p * p <= m; p += 2ULL) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            total *= (seq_t)(e + 1);

            if (p % 3ULL == 2ULL) {
                seq_t E = (seq_t)(e / 2) + 1;        /* #even exponents */
                seq_t O = (seq_t)(e + 1) / 2;        /* #odd exponents  */
                diff *= (E - O);
            } else { /* p % 3 == 1 */
                diff *= (seq_t)(e + 1);
            }
        }
    }

    if (m > 1ULL) {                       /* remaining prime factor */
        uint64_t p = m;
        unsigned e = 1;
        total *= (seq_t)(e + 1);           /* (e+1) = 2 */

        if (p % 3ULL == 2ULL) {
            seq_t E = (seq_t)(e / 2) + 1;   /* E = 1 */
            seq_t O = (seq_t)(e + 1) / 2;   /* O = 1 */
            diff *= (E - O);               /* diff *= 0 */
        } else {                          /* p % 3 == 1 */
            diff *= (seq_t)(e + 1);        /* diff *= 2 */
        }
    }

    /* number of divisors ≡ 1 (mod 6) = (total + diff) / 2 */
    return (total + diff) / 2;
}
