/*
 * A381848  Sequence obtained by replacing 3-term subwords of A010060 by
 *          0,1,2,3,4,5 as described in Comments.
 * https://oeis.org/A381848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   _parity_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A381848
 *
 * a(n) is obtained by taking the 3‑term subword of the Thue‑Morse
 * sequence A010060 starting at position n (n≥1) and replacing it
 * according to the table
 *
 *   0,0,1 → 0
 *   0,1,0 → 1
 *   0,1,1 → 2
 *   1,0,0 → 3
 *   1,0,1 → 4
 *   1,1,0 → 5
 *
 * The Thue‑Morse numbers τ(k) are given by τ(k)=popcount(k‑1) mod 2.
 * In the code the 3 bits are assembled as
 *    bits = (τ(n) << 2) | (τ(n+1) << 1) | τ(n+2)
 * and indexed in a lookup table.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_parity_idx.h"

static const seq_t _tm_map[8] = {
    0, /* 000 – never occurs in Thue–Morse */
    0, /* 001 → 0 */
    1, /* 010 → 1 */
    2, /* 011 → 2 */
    3, /* 100 → 3 */
    4, /* 101 → 4 */
    5, /* 110 → 5 */
    0  /* 111 – never occurs */
};

seq_t a(idx_t n)
{
    /* n is 1‑based. Compute τ(n), τ(n+1), τ(n+2) */
    unsigned t0 = _parity_idx(n - 1); /* τ(n)   */
    unsigned t1 = _parity_idx(n);     /* τ(n+1) */
    unsigned t2 = _parity_idx(n + 1); /* τ(n+2) */

    unsigned bits = (t0 << 2) | (t1 << 1) | t2;
    return (seq_t)_tm_map[bits];
}
