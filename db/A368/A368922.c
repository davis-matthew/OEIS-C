/*
 * A368922  a(n) = n' - n''*2, where n' is the arithmetic derivative of n,
 *          A003415(n) and n'' is the second arithmetic derivative,
 *          A068346(n).
 * https://oeis.org/A368922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 31, i16 n <= 1023, i32 n <= 9552, i64 n <= 9552,
 *            i128 n <= 9552, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A003415(n) - 2*A068346(n)
 *        = arithmetic_derivative(n) - 2*arithmetic_derivative(arithmetic_derivative(n))
 *
 * This implementation computes the arithmetic derivative for
 * unsigned 64‑bit integers by prime factorisation with a small
 * sieve, then applies the formula above.
 *
 * The routine works for any idx_t ≥ 0 and any seq_t type that
 * supports signed integer arithmetic.
 * It is well below the 2 s time limit for n up to 10 000.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

static idx_t *prime_list = NULL;
static size_t prime_count = 0;

/* Initialise a prime table sufficient for all intermediate values
 * that may appear for n ≤ 10 000 (the maximum derivative is < 120 000). */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;                      /* already initialised */

    idx_t limit = 200000;            /* over‑kill limit */
    build_primes(limit, &prime_list, &prime_count);
}

/* Compute the arithmetic derivative of an unsigned 64‑bit integer.
 * n > 1:  d(n) = Σ e_i * n / p_i
 * n ≤ 1:  d(n) = 0
 */
static seq_t arith_derivative_u64(uint64_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t result = 0;
    uint64_t temp = n;

    /* handle factor 2 separately */
    if ((temp & 1ULL) == 0) {
        unsigned exp = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;             /* divide by 2 */
            ++exp;
        }
        result += (seq_t)exp * (seq_t)n / 2;
    }

    /* factorisation by odd primes */
    for (size_t idx = 1; idx < prime_count; ++idx) {   /* prime_list[0] == 2 */
        uint64_t p = (uint64_t)prime_list[idx];
        if (p * p > temp)
            break;
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result += (seq_t)exp * (seq_t)n / (seq_t)p;
        }
    }

    /* any remaining factor > 1 is a prime > sqrt(original temp) */
    if (temp > 1) {
        /* its exponent is 1 */
        result += (seq_t)1 * (seq_t)n / (seq_t)temp;
    }

    return result;
}

/*
 * Public sequence function: a(idx_t n) returns the term a(n).
 */
seq_t a(idx_t n)
{
    /* arithmetic derivative works on unsigned values; cast n accordingly */
    init_primes();

    if (n <= 1)
        return (seq_t)0;

    uint64_t u = (uint64_t)n;
    seq_t d  = arith_derivative_u64(u);
    seq_t d2 = arith_derivative_u64((uint64_t)d);

    return d - (seq_t)2 * d2;
}
