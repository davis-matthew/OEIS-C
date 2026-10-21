/*
 * A272669  A 13-ordering of T = {0,1,2,3,5,8,10,11,12} + 13*Z.
 * https://oeis.org/A272669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 13 * floor(n/9) + d[n mod 9], where d = {0, 1, 2, 3, 5,
 *            8, 10, 11, 12}. This enumerates all integers whose residues
 *            modulo 13 belong to T = {0,1,2,3,5,8,10,11,12}
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t _allowed[9] = {0, 1, 2, 3, 5, 8, 10, 11, 12};

seq_t a(idx_t n)
{
    idx_t full_cycles = n / 9;          /* number of complete 13‑blocks */
    idx_t pos = n % 9;                  /* position within the block   */
    return (seq_t)(full_cycles * 13) + _allowed[pos];
}
