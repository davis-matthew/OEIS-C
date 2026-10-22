/*
 * A386409  Number of nonnegative values s < n such that (-s)^s == -s (mod n).
 * https://oeis.org/A386409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 ≤ s < n : (–s)^s ≡ –s (mod n)}|. The computation uses
 *            modular exponentiation (mod_pow) from the utils. Complexity per
 *            call is O(n log n) for n up to 10 000, well below the 2‑second
 *            limit
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* Handle s = 0 separately – 0^0 is treated as 1; it satisfies the
       congruence only when n = 1. */
    if (n == 1)
        count = 1;

    for (idx_t s = 1; s < n; ++s) {
        /* Compute (–s) mod n, which for 0 < s < n is n - s. */
        seq_t base = (seq_t)(n - s);
        /* Compute base^s mod n.  mod_pow handles any seq_t type. */
        seq_t res  = mod_pow(base, s, (seq_t)n);
        if (res == base)
            ++count;
    }
    return count;
}
