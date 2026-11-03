/*
 * A367918  Numbers formed by concatenating n with the distinct prime factors
 *          of n, left to right, smallest factors to largest, with a(1) = 10.
 * https://oeis.org/A367918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 65, u32 n <= 4289, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9777 (2026-11-01)
 * Helpers:   factor_u64, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A367918.
   For n >= 1, a(n) is formed by writing n in decimal, then appending all
   distinct prime factors of n in increasing order (each factor printed once)
   with no separators.  For n = 1 the convention is a(1) = 10.  */
   
#include <stdint.h>
#include <stdbool.h>

/* External helper: factor_u64(n, primes, exps, &cnt) fills "primes" with
   distinct prime divisors of n and sets *cnt to their number.  */
#include <math.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/num_digits0.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Return the number of decimal digits of a positive integer.  */

/* Return 10^e as a seq_t.  */

seq_t a(idx_t n)
{
    /* Handle the special case required by the OEIS definition. */
    if (n == 1) {
        return (seq_t)10;
    }

    /* Factor n.  For n <= 10 000 the number of distinct primes is small. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Build the concatenated number. */
    seq_t result = (seq_t)n;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        seq_t mult = pow10(num_digits0(p));
        result = result * mult + (seq_t)p;
    }

    return result;
}
