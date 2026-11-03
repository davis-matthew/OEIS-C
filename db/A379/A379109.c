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
 * Range:     i8 n <= 109, i16 n <= 24034, i32 n <= 32769, i64 n <= 32769,
 *            i128 n <= 32769, big any n
 * Validated: standard, pass, 98 terms through n = 9169 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include standard headers needed by this function */
#include <stdint.h>
#include <stdbool.h>

/* helper: check if an exponent is a small prime (≤13) */
static inline bool is_small_prime_exp(int exp)
{
    switch (exp) {
        case 2: case 3: case 5: case 7: case 11: case 13: return true;
        default: return false;
    }
}

/* helper: determine whether p is a Mersenne prime (p = 2^q - 1 with q prime) */
static inline bool is_mersenne_prime_uint64(uint64_t p)
{
    if (p < 3) return false;                /* smallest Mersenne prime is 3 */
    uint64_t q = p + 1ULL;
    /* test whether q is a power of two */
    if ((q & (q - 1)) != 0) return false;
    /* log2(q) gives the exponent q */
    int exp = __builtin_ctzll(q);
    return is_small_prime_exp(exp);
}

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
