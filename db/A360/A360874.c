/*
 * A360874  Number of (non-null) connected induced subgraphs in the 2 X n rook
 *          graph.
 * https://oeis.org/A360874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 3^n + 2^(n+1) - 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t val_four = pow4(n);
    seq_t val_three = pow3(n);
    seq_t val_two = pow2(n + 1);

    seq_t result = val_four - val_three;
    result += val_two;
    result -= 2;
    return result;
}
