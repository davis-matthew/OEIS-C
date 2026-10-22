/*
 * A310523  Coordination sequence Gal.6.355.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4. For n ≥ 1, a(n) = 4 + floor((n-1)/11)*58 +
 *            pref[(n-1) % 11] , where pref =
 *            {0,6,12,18,23,27,32,38,44,50,54,58}. Equivalently, the first
 *            differences a(n)-a(n-1) for n≥2 repeat the period 11 pattern:
 *            6,6,6,5,4,5,6,6,6,4,4. This is the coordination sequence
 *            Gal.6.355.1
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    /* prefix sums of the 11‑term difference pattern */
    static const uint64_t pref[12] = {
        0ULL, 6ULL, 12ULL, 18ULL, 23ULL, 27ULL,
        32ULL, 38ULL, 44ULL, 50ULL, 54ULL, 58ULL
    };

    /* work in a wider type to avoid overflow when seq_t is small */
    uint64_t result = 4ULL;
    idx_t m = n - 1;
    result += (uint64_t)(m / 11) * 58ULL;
    result += pref[m % 11];
    return (seq_t)result;
}
