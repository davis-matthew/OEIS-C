/*
 * A379109  Dirichlet convolution of A046692 (inverse of sigma) with A336923,
 *          where A336923(n) = 1 if sigma(2n) - sigma(n) is a power of 2,
 *          otherwise 0.
 * https://oeis.org/A379109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Sum_{d|n} A046692(d) * A336923(n/d). The function below
 *            implements the multiplicative characterization given in the OEIS
 *            entry
 * Range:     i8 n <= 109, i16 n <= 9169, i32 n <= 9169, i64 n <= 9169,
 *            i128 n <= 9169, big any n
 * Helpers:   is_mersenne_prime_uint64, is_small_prime_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include standard headers needed by this function */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_mersenne_prime_uint64.h"
#include "../utils/is_small_prime_exp.h"

/* multiplicative contribution of a prime power p^e */
static inline seq_t a_prime_power(uint64_t p, int e)
{
    if (p == 2ULL) {
        if (e == 1) return (seq_t)-2;
        else        return (seq_t)0;
    }
    if (is_mersenne_prime_uint64(p)) {
        switch (e) {
            case 1: return -(seq_t)p;
            case 2: return (seq_t)-1;
            case 3: return (seq_t)p;
            default: return (seq_t)0;
        }
    } else {
        switch (e) {
            case 1: return -( (seq_t)p + (seq_t)1 );
            case 2: return (seq_t)p;
            default: return (seq_t)0;
        }
    }
}

/* Main function computing a(n). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of OEIS range, but harmless */

    uint64_t m = (uint64_t)n;            /* work with 64‑bit integer for factoring */
    seq_t result = (seq_t)1;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        seq_t term = a_prime_power(p, exp);
        if (term == (seq_t)0) return (seq_t)0;
        result *= term;
    }

    if (m > 1) {  /* m is now a prime factor */
        seq_t term = a_prime_power(m, 1);
        if (term == (seq_t)0) return (seq_t)0;
        result *= term;
    }

    return result;
}
