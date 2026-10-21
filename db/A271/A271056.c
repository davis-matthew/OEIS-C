/*
 * A271056  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 259", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (n>0?5:0) + 5*floor(n/2) + ( 8*T*(T+1)*(2*T+1)/3 +
 *            12*T*(T+1) - 4*T ), where T = (n>=3)?(n-1)/2:0
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   sum_active_cells
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sum_active_cells.h"

seq_t a(idx_t n) {
    return sum_active_cells(n);
}
