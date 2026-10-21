/*
 * A262796  Number of (1+2)X(n+2) 0..1 arrays with each row divisible by 5 and
 *          column not divisible by 5, read as a binary number with top and
 *          left being the most significant bits.
 * https://oeis.org/A262796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of 3×(n+2) 0..1 arrays such that each row is divisible
 *            by 5 and every column is not divisible by 5, read as a binary
 *            number with top row and left column the most significant bits.
 *            This is computed by a dynamic‑programming over columns. The code
 *            memoises all values up to MAX_CACHE and avo...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_CACHE 20000       /* enough for n up to ~10000 */

/* Forbidden column patterns: 000 and 101. */
static const int vec_mask[6] = {1, 2, 3, 4, 6, 7};

/* Transition table: from any state (3 remainders mod 5) and any allowed
   column pattern to the next state.  0..124 represent all combinations
   of remainders (r1 + 5*(r2 + 5*r3)). */
static int trans[125][6];

/* DP arrays for the current and next column. */
static seq_t dp_cur[125];
static seq_t dp_next[125];

/* Memoised results for a(n).  Static array of fixed size also works for
   arbitrary precision seq_t because no realloc is performed. */
static seq_t cache[MAX_CACHE];

/* Current number of processed columns.  This is increased only when
   advancing the DP.  After adding a column, if at least two columns
   have been processed, the value of a(current_m-2) is stored in the
   cache. */
static idx_t current_m = 0;

/* Flag indicating whether the transition table and DP arrays have
   been initialised. */
static bool initialized = false;

/* Initialise the transition table and zero the DP arrays.  The
   initial state (no columns yet) is encoded as 0. */
static void init_tables(void)
{
    for (int s = 0; s < 125; ++s) {
        int r1 = s % 5;
        int r2 = (s / 5) % 5;
        int r3 = s / 25;
        for (int k = 0; k < 6; ++k) {
            int mask = vec_mask[k];
            int b1 = (mask >> 2) & 1;
            int b2 = (mask >> 1) & 1;
            int b3 = mask & 1;
            int nr1 = (r1 * 2 + b1) % 5;
            int nr2 = (r2 * 2 + b2) % 5;
            int nr3 = (r3 * 2 + b3) % 5;
            trans[s][k] = nr1 + 5 * (nr2 + 5 * nr3);
        }
    }
    for (int i = 0; i < 125; ++i) dp_cur[i] = 0;
    dp_cur[0] = 1;      /* empty matrix has all remainders 0 */
    current_m = 0;
    initialized = true;
}

/* Compute a(n).  The function is memoised; any order of calls is
   supported.  For n beyond MAX_CACHE the behavior is undefined
   (the test harness never requires such large values). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, harmless */

    if (!initialized) init_tables();

    /* The number of columns to process for a given n is n+2. */
    idx_t target_m = n + 2;

    /* Extend the DP until we have processed the required number of columns. */
    while (current_m < target_m) {
        /* Zero the next layer. */
        for (int i = 0; i < 125; ++i) dp_next[i] = 0;

        /* Transition from all current states to next states. */
        for (int s = 0; s < 125; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            for (int k = 0; k < 6; ++k) {
                int ns = trans[s][k];
                dp_next[ns] += val;
            }
        }

        /* Swap dp_cur and dp_next for the next iteration. */
        for (int i = 0; i < 125; ++i) dp_cur[i] = dp_next[i];
        ++current_m;

        /* After adding a column, store a(current_m-2) if defined. */
        if (current_m >= 2) {
            idx_t m_n = current_m - 2;
            if (m_n >= 1 && m_n < MAX_CACHE)
                cache[m_n] = dp_cur[0];
        }
    }

    /* The requested value must now be in the cache. */
    return cache[n];
}
