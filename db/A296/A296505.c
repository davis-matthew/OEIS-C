/*
 * A296505  The least number m not less than the n-th prime p such that m^2 -
 *          p is prime.
 * https://oeis.org/A296505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 49, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The least number m ≥ the n‑th prime p such that m² − p is prime. */

#include <stdbool.h>

/* Helper functions supplied by the build system */
#include "../utils/get_prime.h"      /* uint32_t get_prime(idx_t k);   */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n);    */

seq_t a(idx_t n)
{
    /* Sequence is defined only for n ≥ 1. */
    if (n == 0)
        return 0;  /* undefined, return zero as a safe placeholder */

    /* Get the n‑th prime p.  p fits in 32 bits for all n of interest. */
    seq_t p = (seq_t)get_prime(n);

    /* Search for the smallest m ≥ p with m² – p prime. */
    seq_t m = p;
    for (;; ++m) {
        seq_t d = m * m - p;      /* m² – p */
        if (is_prime_big(d))
            return m;
    }
}
