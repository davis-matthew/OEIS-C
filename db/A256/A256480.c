/*
 * A256480  Smallest prime obtained by appending n to a nonzero number with
 *          identical digits or 0 if no such prime exists.
 * https://oeis.org/A256480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   smallest prime obtained by appending n to a nonzero number with
 *            identical digits; return 0 if no such prime exists (which
 *            happens when n is even or a multiple of 5)
 * Range:     u8 n <= 16, u16 n <= 182, u32 n <= 1288, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_of, is_prime_big, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digits_of.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Immediate cases */
    if ((n % 2 == 0) || (n % 5 == 0))
        return 0;

    /* Number of decimal digits of n */
    idx_t m = digits_of(n);

    /* 10^m as seq_t (needed for concatenation) */
    seq_t pow10m = seq_pow(10, m);

    /* Repunit generator: 1, 11, 111, ... */
    seq_t rep = 0;

    /* Safe maximum length for repunits (works for 64‑bit and 128‑bit seq_t) */
    const idx_t MAX_D = 25;

    for (idx_t d = 1; d <= MAX_D; ++d) {
        rep = rep * 10 + 1;                      /* rep becomes (10^d - 1)/9 */

        for (int k = 1; k <= 9; ++k) {
            seq_t base = rep * (seq_t)k;        /* k repeated d times */

            seq_t candidate = base * pow10m + (seq_t)n; /* base||n */

            if (is_prime_big(candidate))
                return candidate;              /* first prime found is the smallest */
        }
    }

    /* Should never reach here for admissible n, but return 0 as fallback. */
    return 0;
}
