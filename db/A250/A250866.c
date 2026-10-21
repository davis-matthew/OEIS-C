/*
 * A250866  Number of (n+2)X(6+2) 0..3 arrays with nondecreasing sum of every
 *          three consecutive values in every row and column.
 * https://oeis.org/A250866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4, u128 n <= 16, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A250866
    Transformed formulation:
      a(n) = ∏_{p=0}^{2}  ∏_{q=0}^{2}  F( R(p) , C(q) )
    where
      R(p) = ⌊(n+1 – p) / 3⌋ + 1     (rows with i ≡ p (mod 3))
      C(q) = ⌊(7 – q) / 3⌋  + 1      (columns with j ≡ q (mod 3))
    and F(r,c) is the number of r×c arrays of values 0..3 whose rows and
    columns are weakly nondecreasing.  F(r,c) is computed by a dynamic
    programme over the 2‑D product order of the r×c grid.  This gives an
    O(r·c²) algorithm (c ≤ 3) which easily handles n ≤ 10 000.  */

#include <stdint.h>
#include <stdbool.h>

/* -------------  Pre‑computed state tables for c = 2 and 3  ------------- */

#define STATE_MAX2 10   /* C(5,2) */
#define STATE_MAX3 20   /* C(6,3) */

static uint8_t states2[STATE_MAX2][2];
static uint8_t states3[STATE_MAX3][3];
static uint8_t adj2[STATE_MAX2][STATE_MAX2];
static uint8_t adj3[STATE_MAX3][STATE_MAX3];
static int cnt2, cnt3;
static int init_done = 0;

static void init_states(void)
{
    /* generate states for c = 2 */
    cnt2 = 0;
    for (uint8_t a = 0; a <= 3; ++a)
        for (uint8_t b = a; b <= 3; ++b)
        {   states2[cnt2][0] = a;
            states2[cnt2][1] = b;
            ++cnt2;
        }
    for (int i = 0; i < cnt2; ++i)
        for (int j = 0; j < cnt2; ++j)
            adj2[i][j] = (states2[j][0] >= states2[i][0]) &&
                         (states2[j][1] >= states2[i][1]);

    /* generate states for c = 3 */
    cnt3 = 0;
    for (uint8_t a = 0; a <= 3; ++a)
        for (uint8_t b = a; b <= 3; ++b)
            for (uint8_t c = b; c <= 3; ++c)
            {   states3[cnt3][0] = a;
                states3[cnt3][1] = b;
                states3[cnt3][2] = c;
                ++cnt3;
            }
    for (int i = 0; i < cnt3; ++i)
        for (int j = 0; j < cnt3; ++j)
            adj3[i][j] = (states3[j][0] >= states3[i][0]) &&
                         (states3[j][1] >= states3[i][1]) &&
                         (states3[j][2] >= states3[i][2]);
}

/* -------------  Number of weakly nondecreasing r×c arrays  ------------- */

static seq_t compute_F(int r, int c)
{
    if (!init_done) { init_states(); init_done = 1; }

    if (r == 0 || c == 0) return (seq_t)1;

    if (c == 2)
    {
        const int S = STATE_MAX2;
        seq_t dp[S], ndp[S];
        seq_t one = 1, zero = 0;

        /* first row: all states are allowed */
        for (int i = 0; i < S; ++i) dp[i] = one;

        if (r == 1)
        {   seq_t sum = 0;
            for (int i = 0; i < S; ++i) sum += dp[i];
            return sum;
        }

        /* remaining rows */
        for (int row = 2; row <= r; ++row)
        {   for (int j = 0; j < S; ++j) ndp[j] = zero;
            for (int i = 0; i < S; ++i)
            {   if (dp[i] == zero) continue;
                for (int j = 0; j < S; ++j)
                {   if (adj2[i][j]) ndp[j] += dp[i];
                }
            }
            for (int j = 0; j < S; ++j) dp[j] = ndp[j];
        }

        seq_t sum = 0;
        for (int i = 0; i < S; ++i) sum += dp[i];
        return sum;
    }
    else /* c == 3 */
    {
        const int S = STATE_MAX3;
        seq_t dp[S], ndp[S];
        seq_t one = 1, zero = 0;

        for (int i = 0; i < S; ++i) dp[i] = one;

        if (r == 1)
        {   seq_t sum = 0;
            for (int i = 0; i < S; ++i) sum += dp[i];
            return sum;
        }

        for (int row = 2; row <= r; ++row)
        {   for (int j = 0; j < S; ++j) ndp[j] = zero;
            for (int i = 0; i < S; ++i)
            {   if (dp[i] == zero) continue;
                for (int j = 0; j < S; ++j)
                {   if (adj3[i][j]) ndp[j] += dp[i];
                }
            }
            for (int j = 0; j < S; ++j) dp[j] = ndp[j];
        }

        seq_t sum = 0;
        for (int i = 0; i < S; ++i) sum += dp[i];
        return sum;
    }
}

/* --------------------------------------------------------------- */
/*  Main sequence function                                           */
/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* R(p) for p = 0,1,2  (rows of size n+2) */
    int R[3];
    for (int p = 0; p < 3; ++p)
    {   if (p > (int)n + 1) R[p] = 0;
        else                  R[p] = ((int)n + 1 - p) / 3 + 1;
    }

    /* C(q) for q = 0,1,2  (columns of size 8) */
    int C[3];
    for (int q = 0; q < 3; ++q)
    {   if (q > 7) C[q] = 0;
        else       C[q] = (7 - q) / 3 + 1;
    }

    seq_t result = 1;
    for (int p = 0; p < 3; ++p)
        for (int q = 0; q < 3; ++q)
            result *= compute_F(R[p], C[q]);

    return result;
}
