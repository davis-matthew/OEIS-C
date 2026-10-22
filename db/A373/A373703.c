/*
 * A373703  a(n) = A062760(n)*A066636(n).
 * https://oeis.org/A373703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A062760(n)*A066636(n) = radical(n)^(max_exp - min_exp)
 * Range:     u8 n <= 79, u16 n <= 319, u32 n <= 2687, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = (product of distinct primes dividing n)^(max exponent – min exponent)   */
/* For n = 1 the result conventionally equals 1.                                         */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* Definition gives a(1)=1; n==0 is outside the offset (ignored) */
        return 1;
    }

    /* Factor n.  factor_u64 writes primes into primes[], exponents into exps[],
       and the number of prime factors into cnt.  For n up to 64‑bit the table
       of size 64 is sufficient. */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t    cnt   = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the radical (product of distinct primes). */
    seq_t rad = 1;
    uint64_t min_exp = (uint64_t)-1;  /* maximum possible value */
    uint64_t max_exp = 0;

    for (size_t i = 0; i < cnt; ++i) {
        rad *= (seq_t)primes[i];

        if (exps[i] < min_exp) min_exp = exps[i];
        if (exps[i] > max_exp) max_exp = exps[i];
    }

    uint64_t diff = max_exp - min_exp;

    /* If all exponents are equal, result is 1. */
    if (diff == 0) {
        return 1;
    }

    /* Compute rad^diff.  Since diff is typically small (≤64 for 64‑bit n),
       a simple loop suffices and avoids any dependency on pow() or
       exponentiation libraries. */
    seq_t result = 1;
    for (uint64_t i = 0; i < diff; ++i) {
        result *= rad;
    }

    return result;
}
