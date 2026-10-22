/*
 * A340592  a(n) is the concatenation of the prime factors (with multiplicity)
 *          of n mod n.
 * https://oeis.org/A340592
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the prime factors of n (with
 *            multiplicity), interpreted as a decimal integer, taken modulo n
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, digit_count, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the prime‑factor helper _smallest_prime_factor,
   the digit‑count function, and modular exponentiation via mod_pow. */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/digit_count.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    seq_t result = 0;                /* current concatenated value modulo n  */
    seq_t mod = (seq_t)n;            /* modulo value as seq_t               */
    uint64_t tmp = n;                /* working copy of n                   */

    while (tmp > 1) {
        uint64_t p = _smallest_prime_factor(tmp);  /* smallest prime factor */

        /* compute 10^digits(p) mod n once per distinct prime factor */
        unsigned d = digit_count(p);
        seq_t pow10mod = mod_pow((seq_t)10, (uint64_t)d, mod);

        /* repeat for each occurrence of the factor p      */
        while (tmp % p == 0) {
            result = (result * pow10mod + (seq_t)p) % mod;
            tmp /= p;
        }
    }
    return result;    /* already in the required range [0, n-1]          */
}
