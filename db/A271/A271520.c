/*
 * A271520  Domination number of the n-folded cube graph.
 * https://oeis.org/A271520
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = 1 (n=2,3), 2 (n=4), 4 (n=5), 6 (n=6), and 2^(n-4) for n ≥
 *            7
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 6) {
        switch (n) {
            case 2: return (seq_t)1;
            case 3: return (seq_t)1;
            case 4: return (seq_t)2;
            case 5: return (seq_t)4;
            case 6: return (seq_t)6;
            default: return (seq_t)0;   /* should not occur for n≥2 */
        }
    }
    return pow2(n - 4);
}
