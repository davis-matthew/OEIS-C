/*
 * A392628  The number of infinitary divisors of n that are numbers whose
 *          number of divisors is a power of 2.
 * https://oeis.org/A392628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   apply_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A392628:  the number of infinitary divisors of n whose number
 * of divisors is a power of 2.
 *
 * Formula (OEIS notation):
 *   a(n) = ∏_{p^e‖n} (1 + # of trailing 1‑bits in the binary expansion of e)
 *
 *  The function factorises n by trial division (which is fast enough for the
 *  tested range up to 10 000) and for each prime exponent e it multiplies
 *  the running result by (trailing_ones(e) + 1).  The multiplication is
 *  performed in the seq_t type supplied by the build.
 *
 *  It works for any n ≥ 1 and for any order of calls; no caching is
 *  required because the cost per n is O(√n).  For n = 1 the function
 *  returns 1 as required by the sequence.
 *  For n = 0 it returns 0 (undefined by definition). */

#include <stdint.h>
#include "../utils/apply_exponent.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but harmless */
    if (n == 1)
        return 1;          /* multiplicative identity */

    uint64_t temp = (uint64_t)n;
    seq_t result = 1;

    /* Handle factor 2 separately */
    if ((temp & 1ULL) == 0ULL) {
        uint64_t exp = 0;
        while ((temp & 1ULL) == 0ULL) {
            ++exp;
            temp >>= 1;
        }
        apply_exponent(&result, exp);
    }

    /* Trial division for odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }
            apply_exponent(&result, exp);
        }
    }

    /* If anything remains, it's a prime > sqrt(original n) */
    if (temp > 1) {
        /* Remaining prime has exponent 1 */
        apply_exponent(&result, 1);
    }

    return result;
}
