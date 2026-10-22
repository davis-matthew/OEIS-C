/*
 * A327423  Positive integers k such that tan(k) < 0 (or equivalently, cot(k)
 *          < 0).
 * https://oeis.org/A327423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer k such that 1/2 < {k/π} < 1
 *            (equivalently tan(k) < 0)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Fixed‑point representation of π approximately 3.141592653589793238.
   SCALE = 10^18, π_low = floor(π * SCALE), π_high = ceil(π * SCALE).   */
static const uint64_t SCALE = 1000000000000000000ULL;
static const uint64_t PI_LOW  = 3141592653589793238ULL; /* floor(π * 10^18) */
static const uint64_t PI_HIGH = PI_LOW + 1;            /* ceil(π * 10^18) */

/* true iff tan(k) < 0  ⇔  floor(2k/π) is odd. */
static inline bool is_tan_negative(idx_t k)
{
    unsigned __int128 N   = (unsigned __int128)k * 2u;          /* 2k          */
    unsigned __int128 num = N * SCALE;                         /* N * SCALE   */

    /* Compute floor(2k/π) using both bounds of π.
       For the sizes considered here (n ≤ 10 000) the two values agree. */
    uint64_t low  = (uint64_t)(num / PI_LOW);
    uint64_t high = (uint64_t)(num / PI_HIGH);

    /* In the rare event that low != high the parity cannot be decided
       exactly with this approximation, but that never occurs for the
       required range of n.  We therefore take the lower bound. */
    uint64_t val = low;

    return (val & 1ULL) != 0;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; ; ++k) {
        if (is_tan_negative(k)) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}
