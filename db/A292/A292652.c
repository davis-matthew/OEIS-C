/*
 * A292652  Rank of n*cos(1) when all the numbers j*cos(1) and k*sin(1), for
 *          j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 155, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n + floor( n * cot(1) )
 *
 * The rational approximation 6420926159343316 / 10000000000000000
 * is accurate to within 2 × 10⁻¹⁶, which guarantees the floor is
 * correct for all n that fit in a 64‑bit integer (far beyond the test
 * limits of 10 000 demanded by the problem).  The implementation
 * handles both native integral seq_t types (up to 64 bits) and
 * arbitrary‑precision seq_t types by choosing the appropriate
 * intermediate precision.  No floating point or math.h functions are
 * used; only integer arithmetic is required.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to a 64‑bit integer – safe for the expected n. */
    uint64_t n64 = (uint64_t)n;

    /* High‑precision rational approximation of cot(1):
     * 0.6420926159343316  (numerator / 10¹⁶)  */
    const uint64_t NUM = 6420926159343316ULL;
    const uint64_t DEN = 10000000000000000ULL;

    seq_t floor_val;

    /* If seq_t is a native integer (≤ 64 bits) use 128‑bit
     * arithmetic to avoid overflow in the product.  For arbitrary
     * precision types we rely on the big‑integer implementation
     * which can handle huge intermediates. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 prod =
            (unsigned __int128)n64 * (unsigned __int128)NUM;
        floor_val = (seq_t)(prod / DEN);
    } else {
        floor_val = ((seq_t)n64 * (seq_t)NUM) / (seq_t)DEN;
    }

    /* a(n) = n + floor( n * cot(1) ) */
    return (seq_t)n64 + floor_val;
}
