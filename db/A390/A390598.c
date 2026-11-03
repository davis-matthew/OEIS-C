/*
 * A390598  Constant congruence speed in the senary numeral system of the
 *          tetration base n, or -1 if n is a multiple of 6.
 * https://oeis.org/A390598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = - 0 for n == 0 or 1 - -1 for n ≡ 0 (mod 6) - ord₂(n²−1)−1
 *            for n ≡ 3 (mod 6) - ord₃(n²−1) for n ≡ 2 or 4 (mod 6) -
 *            min(ord₂(n²−1)−1, ord₃(n²−1)) for n ≡ 1 or 5 (mod 6)
 * Range:     i8 n <= 12, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 83 (2026-11-01)
 * Helpers:   ctz_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts 0,0,1,2,1,1,-1,1,2,3,2,1,-1,1,1,4,1,2,-1,2,1,2,1,1,… */

#include <stdint.h>
#include "../utils/ctz_seq.h"

/* Return the p‑adic valuation of x for p = 3. */
static inline seq_t ord3(seq_t x)
{
    seq_t c = 0;
    while (x % 3 == 0) {
        ++c;
        x /= 3;
    }
    return c;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Special handling of the first two terms. */
    if (n == 0u || n == 1u) {
        return (seq_t)0;
    }

    /* Terms indexed by multiples of 6 never stabilize. */
    if ((n % 6u) == 0u) {
        return (seq_t)-1;
    }

    /* Compute n² – 1 once, as it is used in several cases. */
    seq_t sq = (seq_t)n * (seq_t)n - (seq_t)1;

    uint8_t mod6 = (uint8_t)(n % 6u);

    /* n ≡ 3 (mod 6) */
    if (mod6 == 3u) {
        return ctz_seq(sq) - (seq_t)1;
    }

    /* n ≡ 2 or 4 (mod 6) */
    if (mod6 == 2u || mod6 == 4u) {
        return ord3(sq);
    }

    /* n ≡ 1 or 5 (mod 6) – use the corrected rule. */
    seq_t v2 = ctz_seq(sq);
    seq_t v3 = ord3(sq);

    /* v2 is at least 3 for these n, so v2 – 1 is safe. */
    seq_t v2_minus1 = v2 - (seq_t)1;

    return (v2_minus1 < v3) ? v2_minus1 : v3;
}
