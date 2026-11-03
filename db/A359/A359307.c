/*
 * A359307  Number of divisors of 6*n-3 of form 6*k+1.
 * https://oeis.org/A359307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9232 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A359307: a(n) = number of divisors of 6*n-3 of the form 6*k+1.
 * The algorithm factorises m = 6*n-3.  
 * Divisors d can contain no factor 2 or 3 (otherwise d≡0,2,3,4,5 mod6)
 * and may use each prime factor p≠2,3 of m.
 * 
 *   †  Prime  p≡1 (mod6) : any exponent 0…e can be chosen – contributes (e+1) possibilities,
 *   †  Prime  p≡5 (mod6) : exponent choices must make the total number of primes with
 *        odd exponent even, because (5*5≡1 mod6).  
 *        For a prime factor with exponent e, let 
 *          e_even = floor(e/2)+1  (exponents that are even)
 *          e_odd  = (e+1)-e_even  (odd exponents)
 *        A dynamic programming over the primes  ≡5 (mod6) keeps two counters
 *          even – number of assignments of processed primes that already yield
 *          an even number of odd exponents,
 *          odd  – number of assignments that yield an odd number.
 *        Updating with a new prime multiplies these counters as:
 *          new_even = even*e_even + odd*e_odd
 *          new_odd  = even*e_odd  + odd*e_even
 *
 * Finally, all choices for primes ≡1 (mod6) are independent of the parity
 * condition, so the total count is 
 *     result = even * product_over_(p≡1) (e+1).
 */

#include <stdint.h>

/* Count divisors of m = 6*n-3 that are 1 (mod 6). */
static inline seq_t count_divs_1mod6(uint64_t m)
{
    /* remove all factors 2 and 3 – a divisor that contains them can
       never be 1 (mod 6) */
    while (m % 2 == 0) m /= 2;
    while (m % 3 == 0) m /= 3;

    /* product of (e+1) for primes p≡1 (mod6) */
    seq_t prod1 = 1;

    /* parity DP: even = assignments with even # of odd exponents
                    odd  = assignments with odd  # of odd exponents */
    seq_t even = 1, odd = 0;

    for (uint64_t d = 5; d * d <= m; d += 6) {
        /* first prime d   (≡5 mod6) */
        uint64_t p = d;
        unsigned e = 0;
        while (m % p == 0) { m /= p; ++e; }
        if (e) {
            seq_t e_even = (seq_t)(e / 2 + 1);     /* even exponents */
            seq_t e_odd  = (seq_t)(e + 1 - e_even);/* odd exponents  */
            seq_t new_even = even * e_even + odd * e_odd;
            seq_t new_odd  = even * e_odd  + odd * e_even;
            even = new_even; odd = new_odd;
        }

        /* second prime d+2 (≡1 mod6) */
        p = d + 2;   /* always prime candidate in 6k+1 */
        e = 0;
        while (m % p == 0) { m /= p; ++e; }
        if (e) prod1 *= (seq_t)(e + 1);
    }

    /* Remaining prime factor, if any */
    if (m > 1) {
        if (m % 6 == 1) {                /* p≡1 (mod6) */
            prod1 *= (seq_t)2;
        } else if (m % 6 == 5) {         /* p≡5 (mod6) */
            unsigned e = 1;
            seq_t e_even = (seq_t)(e / 2 + 1);     /* =1 */
            seq_t e_odd  = (seq_t)(e + 1 - e_even);/* =1 */
            seq_t new_even = even * e_even + odd * e_odd;
            seq_t new_odd  = even * e_odd  + odd * e_even;
            even = new_even; odd = new_odd;
        }
        /* a remaining factor cannot be 0,2,3,4 (mod6) because all those
           factors have already been removed. */
    }

    return even * prod1;
}

seq_t a(idx_t n)
{
    /* m = 6*n - 3 */
    uint64_t m = (uint64_t)n * 6ULL - 3ULL;
    return count_divs_1mod6(m);
}
