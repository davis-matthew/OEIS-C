/*
 * A258091  Smallest prime factor of 1+78557*2^n, cf. A258073.
 * https://oeis.org/A258091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = smallest prime divisor of 1 + 78557·2ⁿ (OEIS A258091) */

#include <stdint.h>
#include <stddef.h>

/* Return the smallest odd prime ≤73 that divides 1+78557*2^n. */
seq_t a(idx_t n)
{
    /* Candidate primes (all odd primes ≤73) – the actual sequence uses only a subset. */
    static const int primes[] = {
        3, 5, 7, 11, 13, 17, 19, 23,
        29, 31, 37, 41, 43, 47, 53, 59,
        61, 67, 71, 73
    };
    const size_t prime_count = sizeof(primes) / sizeof(primes[0]);

    for (size_t i = 0; i < prime_count; ++i) {
        int p = primes[i];
        /* Compute 2^n (mod p) using fast exponentiation, reducing the exponent
           modulo φ(p)=p-1 (Fermat's little theorem). */
        idx_t exp = n % (idx_t)(p - 1);            /* exponent reduced mod p-1  */
        uint64_t base = 2 % p;                     /* current base (mod p)       */
        uint64_t res  = 1 % p;                     /* running result (mod p)     */

        while (exp) {
            if (exp & 1U)
                res = (res * base) % p;
            base = (base * base) % p;
            exp >>= 1U;
        }

        /* Now compute (78557 * 2^n) mod p and test 1 + that value for divisibility. */
        int rem78557 = 78557 % p;
        int term = (int)(( (uint64_t)rem78557 * res ) % p);
        if ( ((term + 1) % p) == 0 )
            return (seq_t)p;                      /* smallest divisor found */
    }

    /* By construction a(n) ≤ 73, so this point is never reached for n ≥ 1. */
    return 0;
}
