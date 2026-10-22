/*
 * A355823  a(n) = 1 if all exponents in prime factorization of n are powers
 *          of 2, otherwise 0.
 * https://oeis.org/A355823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if all exponents in the prime factorization of n are *
 *            powers of 2, otherwise 0. (OEIS A355823)
 * Range:     u8 n <= 9969, u16 n <= 9969, u32 n <= 9969, u64 n <= 9969,
 *            u128 n <= 9969, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_spf.h"

/* static state: smallest prime factor table up to current limit */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Ensure the spf table covers all numbers up to 'limit'. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;
    free(spf);                     /* delete old table if any */
    build_spf(limit, &spf);        /* allocate and fill new table */
    spf_limit = limit;
}

/* Test whether an unsigned integer is a power of two (zero is not). */
static inline int is_power_of_two_uint64(idx_t x)
{
    return x != 0 && (x & (x - 1)) == 0;
}

/* Sequence function.  n >= 1.  For n == 0 the return value is 0
 * (undefined by the sequence offset, but harmless). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* outside defined offset */

    ensure_spf(n);

    idx_t val = n;
    while (val > 1) {
        idx_t p = spf[val];       /* smallest prime factor of current value */
        idx_t exp = 0;
        while (val % p == 0) {
            val /= p;
            ++exp;
        }
        if (!is_power_of_two_uint64(exp))
            return (seq_t)0;      /* exponent not a power of two */
    }
    return (seq_t)1;              /* all exponents are powers of two */
}
