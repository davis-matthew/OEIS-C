/*
 * A262476  Number of (6+1)X(n+1) 0..1 arrays with each row divisible by 3 and
 *          each column divisible by 5, read as a binary number with top and
 *          left being the most significant bits.
 * https://oeis.org/A262476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:
   a(n) = number of (6+1)×(n+1) binary arrays such that every
   of the 7 rows represents a binary number divisible by 3
   and every of the n+1 columns represents a binary number divisible by 5.
   The top row and left column correspond to the most significant bit.
   Implementation uses dynamic programming over columns.  Each state keeps
   the remainders of the 7 rows modulo 3.  There are 3^7 = 2187 possible
   states.  For each column we choose a 7‑bit pattern that is itself
   divisible by 5;  there are 26 such patterns.  Transition tables
   for the two column weights (2^c mod 3 = 1 for even c, 2 for odd c)
   are precomputed once.  The DP iterates over the n+1 columns
   (indexed 0 … n).  The answer is the number of ways to finish all
   rows with remainder 0. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* External integer types provided by the build system */
extern seq_t a(idx_t n);  /* we redeclare only to satisfy the compiler */

#define NUM_ROWS      7
#define NUM_STATES    2187   /* 3^7 */
#define NUM_PAT       26     /* 7‑bit patterns ≡0 (mod 5) */

/* Powers of 3 for decoding/encoding row remainders */
static const int pow3[NUM_ROWS] = {
    1, 3, 9, 27, 81, 243, 729
};

struct seq_cache {
    seq_t cur[NUM_STATES];        /* DP current state counts */
    seq_t nxt[NUM_STATES];        /* DP next state counts   */
    uint8_t state_rem[NUM_STATES][NUM_ROWS];  /* remainder of each row in a state */
    uint8_t pattern_bits[NUM_PAT][NUM_ROWS]; /* the 7‑bit pattern for each column */
    idx_t trans[2][NUM_STATES][NUM_PAT];     /* transition tables for weight 1 and 2 */
};

static struct seq_cache *cache = NULL;

/* Compute all column patterns that are ≡0 (mod 5) */
static void generate_patterns(void)
{
    idx_t pat_idx = 0;
    for (int v = 0; v < 128; ++v) {
        if (v % 5 != 0) continue;          /* pattern must be divisible by 5 */
        for (int r = 0; r < NUM_ROWS; ++r)
            cache->pattern_bits[pat_idx][r] = (v >> (6 - r)) & 1;
        ++pat_idx;
    }
}

/* Decode each state into its 7 remainders modulo 3 */
static void decode_states(void)
{
    for (idx_t s = 0; s < NUM_STATES; ++s) {
        for (int r = 0; r < NUM_ROWS; ++r)
            cache->state_rem[s][r] = (s / pow3[r]) % 3;
    }
}

/* Precompute transition tables for column weights 1 and 2 */
static void compute_transitions(void)
{
    for (int wIdx = 0; wIdx < 2; ++wIdx) {
        int weight = (wIdx == 0) ? 1 : 2;          /* 2^c mod 3 : odd →2 */
        for (idx_t s = 0; s < NUM_STATES; ++s) {
            for (idx_t p = 0; p < NUM_PAT; ++p) {
                int t = 0;
                for (int r = 0; r < NUM_ROWS; ++r) {
                    int rem  = cache->state_rem[s][r];
                    int add  = cache->pattern_bits[p][r] * weight;
                    int nrem = (rem + add) % 3;
                    t += nrem * pow3[r];
                }
                cache->trans[wIdx][s][p] = t;
            }
        }
    }
}

/* Lazy initialization of the cache */
static void init_cache(void)
{
    if (cache) return;                       /* already initialized */

    cache = (struct seq_cache*)calloc(1, sizeof(*cache));
    if (!cache) abort();   /* in case of allocation failure */

    generate_patterns();
    decode_states();
    compute_transitions();
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1, but defined for safety */
    init_cache();

    seq_t *cur = cache->cur;
    seq_t *nxt = cache->nxt;

    /* initialize DP */
    memset(cur, 0, sizeof(seq_t) * NUM_STATES);
    cur[0] = (seq_t)1;          /* all row remainders initially 0 */

    /* iterate over the n+1 columns (indices 0 … n) */
    for (idx_t col = 0; col <= n; ++col) {
        int wIdx = (col & 1) ? 1 : 0;          /* even column → weight 1, odd → 2 */
        /* reset nxt */
        memset(nxt, 0, sizeof(seq_t) * NUM_STATES);

        for (idx_t s = 0; s < NUM_STATES; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0) continue;
            for (idx_t p = 0; p < NUM_PAT; ++p) {
                idx_t t = cache->trans[wIdx][s][p];
                nxt[t] += val;
            }
        }
        /* swap buffers */
        memcpy(cur, nxt, sizeof(seq_t) * NUM_STATES);
    }
    return cur[0];     /* all rows must have remainder 0 after last column */
}
