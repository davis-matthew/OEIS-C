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
 * Helpers:   digit_count, factor_u64, pow10
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
#include "../utils/factor_u64.h"

/* Return the number of decimal digits of a positive integer.  */
#include "../utils/digit_count.h"

/* Return 10^e as a seq_t.  */
#include "../utils/pow10.h"

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
        seq_t mult = pow10(digit_count(p));
        result = result * mult + (seq_t)p;
    }

    return result;
}
