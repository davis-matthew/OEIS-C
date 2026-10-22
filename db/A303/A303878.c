/*
 * A303878  Consider the representation of some integer (>1) as the sum of
 *          distinct unit fraction (<1). The sum of these denominators is
 *          least.
 * https://oeis.org/A303878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = nth denominator in the minimal sum-of-denominators
 *            representation of 2 as a sum of distinct unit fractions (<1).
 *            The optimal representation is 2 = 2⁻¹ + 3⁻¹ + 4⁻¹ + 5⁻¹ + 6⁻¹ +
 *            8⁻¹ + 9⁻¹ + 10⁻¹ + 15⁻¹ + 18⁻¹ + 20⁻¹ + 24⁻¹. The denominators
 *            of this representation, sorted ascending, form the...
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Static array of the 12 terms.  seq_t may be any unsigned integer type. */
    static const seq_t table[12] = {
        (seq_t)2,
        (seq_t)3,
        (seq_t)4,
        (seq_t)5,
        (seq_t)6,
        (seq_t)8,
        (seq_t)9,
        (seq_t)10,
        (seq_t)15,
        (seq_t)18,
        (seq_t)20,
        (seq_t)24
    };
    if (n > 0 && n <= 12)
        return table[n - 1];
    /* For n beyond the known terms, return 0 (undefined). */
    return (seq_t)0;
}
