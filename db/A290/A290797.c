/*
 * A290797  Coefficients in 7-adic expansion of sqrt(-3).
 * https://oeis.org/A290797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth coefficient (0‑based) in the 7‑adic expansion of
 *            √‑3
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   grow_size
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses Hensel lifting.  For the current approximation
   x_m = Σ_{i=0}^{m-1} a[i]·7^i  (mod 7^m) we compute the next digit a[m]
   as   a[m] ≡ – ((x_m² + 3)/7^m)·(2·a[0])⁻¹  (mod 7).
   Here (2·a[0])⁻¹ mod 7 = 5 because a[0] = 5.   */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/grow_size.h"

static seq_t *digits = NULL;          /* a[i]   */
static uint64_t *raw = NULL;          /* raw[i] : coefficient before carrying, can be large */
static size_t  d_cap = 0;             /* allocated size for digits   */
static size_t  r_cap = 0;             /* allocated size for raw     */
static idx_t   last_computed = (idx_t)(-1);

seq_t a(idx_t n)
{
    /* ----- initialisation (first call) ----- */
    if (last_computed == (idx_t)(-1)) {
        /* allocate digits array */
        grow_size(&d_cap, (void **)&digits, sizeof(seq_t), (size_t)n+1);
        digits[0] = (seq_t)5;          /* a(0) = 5   */
        last_computed = 0;

        /* allocate raw coefficient array; we need indices up to 2*n */
        grow_size(&r_cap, (void **)&raw, sizeof(uint64_t), 2*(size_t)n + 1);
        raw[0] = (uint64_t)25;         /* 5^2 = 25   */
        /* other raw entries are zero by zeroed allocation */
    }

    /* grow arrays if the caller requested a larger n */
    if ((size_t)n >= d_cap) {
        grow_size(&d_cap, (void **)&digits, sizeof(seq_t), (size_t)n + 1);
    }
    if (2*(size_t)n + 1 >= r_cap) {
        grow_size(&r_cap, (void **)&raw, sizeof(uint64_t), 2*(size_t)n + 1);
    }

    /* ----- generate digits up to n ----- */
    for (idx_t m = last_computed + 1; m <= n; ++m) {
        /* m = 0 gives the already known a(0); skip the computation code below. */
        if (m == 0) continue;

        /* ----- compute k = ((x_m^2 + 3) / 7^m) mod 7 ----- */
        uint64_t carry = 3;                     /* +3 at the 7^0 place */
        for (idx_t i = 0; i < m; ++i) {
            carry = (carry + raw[i]) / 7;
        }
        uint64_t k = (raw[m] + carry) % 7;

        /* ----- compute the new digit a[m] ----- */
        const uint64_t inv_two_a0 = 5;          /* (2*a[0])⁻¹ mod 7 */
        uint8_t a_m = (uint8_t)(( (7 - k) * inv_two_a0 ) % 7);
        digits[m] = (seq_t)a_m;

        /* ----- update raw coefficients for the square of the new x ----- */
        for (idx_t i = 0; i < m; ++i) {
            raw[i + m] += 2ULL * a_m * (uint64_t)digits[i];
        }
        raw[2 * m] += (uint64_t)a_m * (uint64_t)a_m;
        /* no need to carry‑reduce raw: the algorithm for k uses raw
           values directly via integer division, so raw can grow large
           but stays well below 2^64 for the range of n we expect. */
    }
    last_computed = n;

    return digits[n];
}
