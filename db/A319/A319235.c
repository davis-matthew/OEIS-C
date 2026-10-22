/*
 * A319235  The profile of the backtrack tree for the eight queens problem.
 * https://oeis.org/A319235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of nodes at depth n in Knuth's backtrack tree for
 *            the 8‑queens problem. a(0)=1, a(1)=8, a(2)=42, a(3)=140,
 *            a(4)=344, a(5)=568, a(6)=550, a(7)=312, a(8)=92, a(n)=0 for n>8
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static seq_t profile[9];
static bool initialized = false;
static const int BOARD = 8;
static const uint64_t ALLCOLS = (1ULL << BOARD) - 1;

/* Recursive backtrack to count nodes per depth. */
static void dfs(uint64_t cols, uint64_t diag1, uint64_t diag2,
                int depth, seq_t *prof)
{
    if (depth == BOARD) return;          /* all queens placed */

    uint64_t free = (~(cols | diag1 | diag2)) & ALLCOLS;
    while (free) {
        uint64_t pos = free & -free;     /* least significant free bit */
        free -= pos;

        prof[depth + 1]++;               /* node at next depth */

        uint64_t new_cols = cols | pos;
        uint64_t new_diag1 = ((diag1 | pos) << 1) & ALLCOLS;
        uint64_t new_diag2 = (diag2 | pos) >> 1;

        dfs(new_cols, new_diag1, new_diag2, depth + 1, prof);
    }
}

/* Build the profile once. */
static void compute_profile(void)
{
    for (int i = 0; i < 9; ++i) profile[i] = 0;
    profile[0] = 1;                       /* root node */
    dfs(0, 0, 0, 0, profile);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) compute_profile();

    if (n < 9) return profile[(int)n];
    return 0;
}
