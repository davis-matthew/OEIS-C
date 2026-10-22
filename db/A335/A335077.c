/*
 * A335077  a(n) sets a record for side length k of zero-triangle in a
 *          rotationally symmetrical XOR-triangle.
 * https://oeis.org/A335077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5 + 7*4^n)/3 for n > 4; a(1)=1, a(2)=11, a(3)=39,
 *            a(4)=543, a(5)=2391, a(6)=9559
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n <= 6) {
        switch ((idx_t)n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)11;
            case 3: return (seq_t)39;
            case 4: return (seq_t)543;
            case 5: return (seq_t)2391;
            case 6: return (seq_t)9559;
            default: break;
        }
    }
    seq_t pow4_n = pow4(n);
    seq_t num = pow4_n * ((seq_t)7) + ((seq_t)5);
    return num / ((seq_t)3);
}
