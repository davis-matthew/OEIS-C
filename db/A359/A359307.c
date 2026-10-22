/*
 * A359307  Number of divisors of 6*n-3 of form 6*k+1.
 * https://oeis.org/A359307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_divs_1mod6
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
#include "../utils/count_divs_1mod6.h"

seq_t a(idx_t n)
{
    /* m = 6*n - 3 */
    uint64_t m = (uint64_t)n * 6ULL - 3ULL;
    return count_divs_1mod6(m);
}
