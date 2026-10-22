/*
 * A392654  Binary-encoded contents of pin 3, after n moves, when moving
 *          infinitely many disks in the Tower of Hanoi starting from pin 1,
 *          via the optimal algorithm.
 * https://oeis.org/A392654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A392654 – Binary-encoded contents of pin 3 after n optimal
 * Tower‑of‑Hanoi moves (infinitely many disks).  For each disk k (0‑based)
 * let M_k(n) be the number of times disk k has moved after n moves.
 *   M_k(n) = floor((n + 2^k)/2^(k+1))
 * Disk k moves clockwise if k is even, counter‑clockwise if k is odd.
 * The peg index after n moves is
 *   p_k(n) = (M_k(n) * d_k) mod 3              (d_k = 1 if k even, 2 if k odd)
 * If p_k(n) == 2 (pin 3) we set bit k in the result.
 * The function runs in O(log n) time and uses only integer arithmetic.
 */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* iterate over disks that have moved at least once */
    for (idx_t k = 0; ( (idx_t)1 << k ) <= n; ++k)
    {
        /* number of full cycles of length 2^(k+1) in n */
        idx_t full = (k + 1 < 64) ? n >> (k + 1) : 0;
        /* remainder part */
        idx_t rem  = (k + 1 < 64) ?
                     ( n & (((idx_t)1 << (k + 1)) - 1) ) : n;
        /* did the partial cycle cross the first 2^k moves? */
        idx_t add  = (rem >= ((idx_t)1 << k)) ? 1 : 0;
        idx_t moves = full + add;                /* M_k(n) */

        /* direction: 1 for clockwise (even), 2 for counter‑clockwise (odd) */
        unsigned dir = (k & 1) ? 2 : 1;
        unsigned peg = (unsigned)((moves % 3) * dir % 3);

        if (peg == 2)                     /* disk k is on pin 3 */
        {
            result |= ( (seq_t)1 << k );
        }
    }
    return result;
}
