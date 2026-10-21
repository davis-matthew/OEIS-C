/*
 * A251316  Number of (n+1)X(7+1) 0..1 arrays with no 2X2 subblock having the
 *          maximum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A251316
    a(n) = number of (n+1)×8 binary arrays with no 2×2 subblock
    whose maximum diagonal element exceeds the absolute difference
    of its anti‑diagonal elements.  The condition is equivalent to
    forbidding any 2×2 block in which a diagonal entry is 1 and the
    two anti‑diagonal entries are equal.  The function computes the
    count by dynamic programming over 8‑bit row states. */

#include <stdint.h>

/* Compatibility test for two consecutive rows.
   Returns 1 if the pair (top,bottom) is allowed, 0 otherwise. */
static inline uint8_t compatible(uint8_t top, uint8_t bottom)
{
    for (uint8_t col = 0; col < 7; ++col)
    {
        uint8_t a0 = (top    >> col) & 1;   /* top‑left     */
        uint8_t a1 = (top    >> (col+1)) & 1; /* top‑right  */
        uint8_t b0 = (bottom >> col) & 1;   /* bottom‑left  */
        uint8_t b1 = (bottom >> (col+1)) & 1; /* bottom‑right*/
        if ((a0 | b1) && (a1 == b0))        /* forbidden pattern   */
            return 0;
    }
    return 1;
}

/*  Main sequence function.  --------------------------------- */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) corresponds to 2 rows.  */
    if (n == 0) return 0;

    /* Pre‑computed adjacency matrix 256×256.  */
    static uint8_t adj[256][256];
    static int ready = 0;
    if (!ready)
    {
        for (uint16_t i = 0; i < 256; ++i)
            for (uint16_t j = 0; j < 256; ++j)
                adj[i][j] = compatible((uint8_t)i, (uint8_t)j);
        ready = 1;
    }

    /* DP arrays: count of ways to reach each row state at current
       height.  */
    static seq_t cur[256];
    static seq_t nxt[256];

    /* Initial distribution: any first row is allowed.  */
    for (uint16_t i = 0; i < 256; ++i) cur[i] = 1;

    /* Perform n transitions (between the n+1 rows).  */
    for (idx_t step = 0; step < n; ++step)
    {
        for (uint16_t j = 0; j < 256; ++j) nxt[j] = 0;
        for (uint16_t i = 0; i < 256; ++i)
        {
            seq_t val = cur[i];
            if (val == 0) continue;
            for (uint16_t j = 0; j < 256; ++j)
                if (adj[i][j])
                    nxt[j] += val;
        }
        /* prepare for next iteration */
        for (uint16_t i = 0; i < 256; ++i) cur[i] = nxt[i];
    }

    /* Sum counts over all possible final rows.  */
    seq_t total = 0;
    for (uint16_t i = 0; i < 256; ++i) total += cur[i];
    return total;
}
