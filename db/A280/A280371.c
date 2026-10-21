/*
 * A280371  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 260", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A280371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   For n ≤ 12 the values are given explicitly. * For n ≥ 13, * let
 *            r = n mod 4 * baseIndex = {12, 9, 10, 11}[r] // 12→a(12),
 *            9→a(9), 10→a(10), 11→a(11) * a(n) = a(baseIndex) * 10000^((n -
 *            baseIndex)/4) * where a(k) for k ≤ 12 is obtained from the
 *            initial table. * (The exponent is an integer ≥ 0.) *...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow_uint, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_from_str.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Initial values for n = 0 … 12 */
    static seq_t init[13];
    static bool built = false;
    if (!built) {
        init[0]  = seq_from_str("1");
        init[1]  = seq_from_str("10");
        init[2]  = seq_from_str("101");
        init[3]  = seq_from_str("1000");
        init[4]  = seq_from_str("10000");
        init[5]  = seq_from_str("101000");
        init[6]  = seq_from_str("1010100");
        init[7]  = seq_from_str("10000010");
        init[8]  = seq_from_str("100000001");
        init[9]  = seq_from_str("1010000000");
        init[10] = seq_from_str("10101000000");
        init[11] = seq_from_str("100000100000");
        init[12] = seq_from_str("1000000000000");
        built = true;
    }

    if (n <= 12) {
        return init[(size_t)n];
    }

    /* n >= 13 */
    idx_t r = n % 4;
    /* Mapping of residue to the base index used for the recurrence */
    static const idx_t baseIndexTable[4] = {12, 9, 10, 11};
    idx_t baseIndex = baseIndexTable[r];

    /* Power of 10000 to multiply */
    idx_t power = (n - baseIndex) / 4;

    seq_t base = init[baseIndex];
    seq_t factor = pow_uint(10000, power);

    return base * factor;
}
