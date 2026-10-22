/*
 * A350703  a(n) is the least integer k such that (2*n*k+1) | (2^k-1).
 * https://oeis.org/A350703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that (2*n*k+1) divides (2^k-1)
 * Range:     u8 n <= 26, u16 n <= 1237, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   modpow_leads_to_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation performs a straightforward search over k.  It uses
   128‑bit integer arithmetic to avoid overflow when the modulus
   2*n*k+1 exceeds the width of seq_t.  The algorithm is correct for all
   indices that fit within the computational limits stated in the
   requirements. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/modpow_leads_to_one.h"

seq_t a(idx_t n)
{
    for (idx_t k = 1; ; ++k) {
        if (modpow_leads_to_one(n, k))
            return (seq_t)k;
    }
}
