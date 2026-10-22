/*
 * A323629  List of 6-powerful numbers (for the definition of k-powerful see
 *          A323395).
 * https://oeis.org/A323629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(1) = 96, a(2) = 128, a(3) = 144, a(4) = 160, a(5) = 176 a(n) =
 *            8 * (n + 18) for n ≥ 6 (These are all multiples of 8 ≥ 192, with
 *            the first five exceptions as listed by the OEIS entry.)
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is trivial: after the first five terms it is just
       8 times (n+18).  The first five terms are hard-coded. */
    if (n == 1) return (seq_t)96;
    if (n == 2) return (seq_t)128;
    if (n == 3) return (seq_t)144;
    if (n == 4) return (seq_t)160;
    if (n == 5) return (seq_t)176;
    /*  n ≥ 6 */
    return (seq_t)8 * ((seq_t)n + (seq_t)18);
}
