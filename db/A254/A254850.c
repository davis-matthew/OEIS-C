/*
 * A254850  Number of (n+2) X (4+2) 0..1 arrays with no 3 X 3 subblock
 *          diagonal sum one and no antidiagonal sum two.
 * https://oeis.org/A254850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 15, u64 n <= 37, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A254850 counts the number of (n+2)×6 binary matrices
   in which every 3×3 sub‑block satisfies:
      • the sum of the main diagonal ≠ 1  (i.e. 0, 2 or 3)
      • the sum of the anti‑diagonal ≠ 2  (i.e. 0, 1, 3 or 4)

   For each n ≥ 1 we build a matrix with rows = n+2 and return the
   number of matrices.  The implementation uses a dynamic programme
   over the last two rows.  There are 2¹² = 4096 states (each state
   encodes two consecutive 6‑bit rows).  For every state we pre‑compute
   the mask of admissible next rows (≤ 64 of them).  Then we iteratively
   extend the matrix one row at a time.

   This algorithm is O(n·4096) time with ~ 10 k operations per row
   and uses a few kilobytes of memory.  It is valid C11/C++17 and
   uses only operations allowed by seq_t. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

static uint64_t allow_mask[4096];
static bool mask_inited = false;

static inline bool valid_row(uint8_t pp, uint8_t p, uint8_t c)
{
    /* pp – row r-2  (6 bits)
       p  – row r-1  (6 bits)
       c  – row r    (6 bits)   */
    for (int j = 0; j < 4; ++j) {
        int a = (pp >>  j)          & 1;
        int b = (p  >> (j+1))        & 1;
        int d = (c  >> (j+2))        & 1;
        if ((a + b + d) == 1) return false;
        int e = (pp >> (j+2))        & 1;
        int f = (p  >> (j+1))        & 1;
        int g = (c  >>  j)           & 1;
        if ((e + f + g) == 2) return false;
    }
    return true;
}

static void init_masks(void)
{
    for (int s = 0; s < 4096; ++s) {
        uint8_t pp = (s >> 6) & 0x3F;   /* previous two rows */
        uint8_t p  =  s        & 0x3F;
        uint64_t mask = 0;
        for (int c = 0; c < 64; ++c) {
            if (valid_row(pp, p, (uint8_t)c))
                mask |= (1ULL << c);
        }
        allow_mask[s] = mask;
    }
    mask_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* unused, but harmless */

    if (!mask_inited) init_masks();

    const idx_t rows = n + 2;          /* total number of rows in the matrix */
    static seq_t cur[4096];
    static seq_t nxt[4096];
    /* initialise with the two first rows – any configuration is allowed */
    for (idx_t i = 0; i < 4096; ++i) cur[i] = (seq_t)1;

    for (idx_t r = 3; r <= rows; ++r) {
        memset(nxt, 0, sizeof(nxt));
        for (idx_t s = 0; s < 4096; ++s) {
            if ((seq_t)0 == cur[s]) continue;
            uint64_t mask = allow_mask[s];
            while (mask) {
                /* extract least significant set bit */
                uint64_t lz = mask & -mask;
                int newrow = (int)(__builtin_ctzll(mask));
                idx_t newstate = ((s & 0x3F) << 6) | newrow;
                nxt[newstate] += cur[s];
                mask ^= lz;
            }
        }
        /* after processing the last row, compute the answer */
        if (r == rows) {
            seq_t total = (seq_t)0;
            for (idx_t i = 0; i < 4096; ++i) total += nxt[i];
            return total;
        }
        /* prepare for next iteration */
        memcpy(cur, nxt, sizeof(cur));
    }
    return (seq_t)0;  /* should never reach here */
}
