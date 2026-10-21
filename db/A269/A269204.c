/*
 * A269204  Number of 4Xn 0..3 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling three no more than once.
 * https://oeis.org/A269204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of 4×n arrays of digits 0..3 in which at most one
 *  adjacent pair (horizontally, diagonally or anti‑diagonally) sums to 3.  */

#include <stdint.h>
#include <string.h>

/* Cached transition data -------------------------------------------*/
static int     decode[256][4];
static uint8_t pair_cnt[256][256];
static uint8_t zero_len[256];
static uint8_t one_len[256];
static uint8_t zero_list[256][256];
static uint8_t one_list[256][256];
static int initialized = 0;

/* Build lookup tables.  Run only once. */
static void init_tables(void)
{
    /* Decode each 4‑digit column (base‑4). */
    for (int m = 0; m < 256; ++m) {
        int x = m;
        for (int r = 0; r < 4; ++r) {  /* least significant row first */
            decode[m][r] = x & 3;
            x >>= 2;
        }
    }

    /* Count “sum‑3” adjacent pairs across two columns. */
    for (int j = 0; j < 256; ++j) {
        for (int k = 0; k < 256; ++k) {
            int cnt = 0;
            for (int r = 0; r < 4; ++r) {
                int a = decode[j][r];
                int b = decode[k][r];
                if (a + b == 3) ++cnt;          /* horizontal            */
                if (r > 0) {                     /* diagonal (up-right)   */
                    int b2 = decode[k][r - 1];
                    if (a + b2 == 3) ++cnt;
                }
                if (r < 3) {                     /* antidiagonal (down‑right) */
                    int b3 = decode[k][r + 1];
                    if (a + b3 == 3) ++cnt;
                }
            }
            pair_cnt[j][k] = (uint8_t)cnt;
        }
    }

    /* For each previous column build lists of successors that add
     * 0 or exactly 1 new sum‑3 pair. */
    for (int j = 0; j < 256; ++j) {
        uint8_t z = 0, o = 0;
        for (int k = 0; k < 256; ++k) {
            uint8_t pc = pair_cnt[j][k];
            if (pc == 0) {
                zero_list[j][z++] = (uint8_t)k;
            } else if (pc == 1) {
                one_list[j][o++] = (uint8_t)k;
            }
            /* pc > 1 are ignored – such transitions are forbidden. */
        }
        zero_len[j] = z;
        one_len[j]  = o;
    }
    initialized = 1;
}

/* ------------------------------------------------------------------- */
/*  Main routine.  Implements dynamic programming over columns.        */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined below offset, but safe */

    if (!initialized) init_tables();

    /* n = 1: 4^4 = 256, no cross‑column adjacencies possible. */
    if (n == 1) return (seq_t)256;

    /* DP tables:  dp0/1 hold counts for current column
     *  before the next transition.  Indices are column codes 0..255.
     */
    seq_t dp0[256];
    seq_t dp1[256];
    seq_t next0[256];
    seq_t next1[256];

    /* Initialise: any column works, no sum‑3 pair seen yet. */
    for (int i = 0; i < 256; ++i) {
        dp0[i] = (seq_t)1;
        dp1[i] = (seq_t)0;
    }

    /* Iterate over the remaining columns. */
    for (idx_t step = 1; step < n; ++step) {
        /* Zero next arrays. */
        for (int i = 0; i < 256; ++i) {
            next0[i] = (seq_t)0;
            next1[i] = (seq_t)0;
        }

        /* Perform transitions from each column. */
        for (int j = 0; j < 256; ++j) {
            seq_t cur0 = dp0[j];
            seq_t cur1 = dp1[j];

            if (cur0) {
                /* Zero‑pair successors – flag stays 0. */
                uint8_t zlen = zero_len[j];
                for (int t = 0; t < zlen; ++t) {
                    int k = zero_list[j][t];
                    next0[k] += cur0;
                }
                /* One‑pair successors – set flag to 1. */
                uint8_t olen = one_len[j];
                for (int t = 0; t < olen; ++t) {
                    int k = one_list[j][t];
                    next1[k] += cur0;
                }
            }
            if (cur1) {
                /* Zero‑pair successors – flag remains 1. */
                uint8_t zlen = zero_len[j];
                for (int t = 0; t < zlen; ++t) {
                    int k = zero_list[j][t];
                    next1[k] += cur1;
                }
                /* One‑pair successors are forbidden when flag is already set. */
            }
        }

        /* Move to next step.  (No memcpy – copies are performed element‑wise.) */
        for (int i = 0; i < 256; ++i) {
            dp0[i] = next0[i];
            dp1[i] = next1[i];
        }
    }

    /* Sum all final states, irrespective of flag. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 256; ++i) {
        total += dp0[i];
        total += dp1[i];
    }
    return total;
}
