/*
 * A263460  Number of (n+1)X(2+1) 0..4 arrays with each row and column
 *          divisible by 11, read as a base-5 number with top and left being
 *          the most significant digits.
 * https://oeis.org/A263460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of (n+1)×3 arrays with digits 0..4 such that each * row
 *            and each column (interpreted as base‑5 numbers with the *
 *            leftmost/topmost digit most significant) is divisible by 11
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm keeps a dynamic programming table over the column sums
 * modulo 11.  For each row the possible digit triples that satisfy the
 * row divisibility condition are precomputed, and for each possible
 * weight (5^k mod 11) we precompute the resulting column sum
 * increments.  The table of size 11³=1331 is updated row by row.
 *
 * Complexity:  O( (n+1) · 1331 · R )  where R ≈ 12 is the number
 * of admissible digit triples.  For n≤10000 this runs comfortably
 * within the time limit.  All arithmetic is performed using the
 * seq_t type which can be an arbitrary‑precision integer. */

#include <stdint.h>
#include <stddef.h>

#define NUM_STATES (11*11*11)
#define MAX_TRIPS 125

/* Powers 5^k mod 11 for k = 0..4 (cycle length 5) */
static const int pow5_mod11[5] = {1, 5, 3, 4, 9};

/* Row digit triples that satisfy 3a + 5b + c ≡ 0 (mod 11) */
typedef struct { uint8_t a,b,c; } RowTrip;
static RowTrip row_triples[MAX_TRIPS];
static int row_count = 0;

/* For each weight w (0..10) the list of column‑sum increments.
 * Each increment is a 3‑tuple (d0,d1,d2) where di = (w * di_row) mod 11. */
typedef struct { uint8_t d0,d1,d2; } IncTrip;
static IncTrip inc_trips[11][MAX_TRIPS];
static int inc_count[11];

/* One‑time initialization of the tables */
static void init_tables(void)
{
    if (row_count > 0) return;  /* already done */

    /* Collect admissible row triples */
    for (int a = 0; a <= 4; ++a)
        for (int b = 0; b <= 4; ++b)
            for (int c = 0; c <= 4; ++c)
                if ( (3*a + 5*b + c) % 11 == 0 )
                    row_triples[row_count++] = (RowTrip){(uint8_t)a,
                                                         (uint8_t)b,
                                                         (uint8_t)c};

    /* Precompute increments for each weight */
    for (int w = 0; w <= 10; ++w) {
        int cnt = 0;
        for (int k = 0; k < row_count; ++k) {
            int d0 = (w * row_triples[k].a) % 11;
            int d1 = (w * row_triples[k].b) % 11;
            int d2 = (w * row_triples[k].c) % 11;
            inc_trips[w][cnt++] = (IncTrip){(uint8_t)d0,
                                            (uint8_t)d1,
                                            (uint8_t)d2};
        }
        inc_count[w] = cnt;
    }
}

/* Helper: map 3D indices to single array index */
static inline int state_index(unsigned s0, unsigned s1, unsigned s2)
{
    return (s0 * 121) + (s1 * 11) + s2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_tables();

    /* Number of rows is n+1 (offset 1 gives 2×3 array for n=1) */
    idx_t rows = n + 1;

    /* Allocate DP tables on the stack */
    seq_t dp[NUM_STATES];
    seq_t newdp[NUM_STATES];

    /* Initialise dp: zero all states except the zero‑state */
    for (int i = 0; i < NUM_STATES; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Process each row */
    for (idx_t r = 0; r < rows; ++r) {
        int w = pow5_mod11[( (n - r) % 5 )];

        /* Zero newdp */
        for (int i = 0; i < NUM_STATES; ++i)
            newdp[i] = (seq_t)0;

        /* Update DP */
        for (unsigned s0 = 0; s0 < 11; ++s0)
            for (unsigned s1 = 0; s1 < 11; ++s1)
                for (unsigned s2 = 0; s2 < 11; ++s2) {
                    int idx = state_index(s0, s1, s2);
                    seq_t cur = dp[idx];
                    if (cur == (seq_t)0) continue;

                    int incs = inc_count[w];
                    for (int k = 0; k < incs; ++k) {
                        unsigned ns0 = (s0 + inc_trips[w][k].d0) % 11;
                        unsigned ns1 = (s1 + inc_trips[w][k].d1) % 11;
                        unsigned ns2 = (s2 + inc_trips[w][k].d2) % 11;
                        int nidx = state_index(ns0, ns1, ns2);
                        newdp[nidx] += cur;
                    }
                }

        /* Move to next row */
        for (int i = 0; i < NUM_STATES; ++i)
            dp[i] = newdp[i];
    }

    return dp[0];
}
