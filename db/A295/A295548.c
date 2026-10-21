/*
 * A295548  Number of nX5 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 1, 3 or 4 1s.
 * https://oeis.org/A295548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the A295548 sequence:
   Number of n×5 binary arrays in which every 1 has exactly 1,
   3 or 4 orthogonal 1‑neighbours (horizontal or vertical).  It uses
   dynamic programming over 5‑bit row states and a 3‑row sliding window. */

#include <stdbool.h>

/* 5‑bit mask width */
#define WIDTH 5
#define STATES (1U << WIDTH)

/* 3‑row validity table: valid[prev][curr][next] */
static bool valid[STATES][STATES][STATES];
static bool initialized = false;

/* Pre‑compute the validity table once. */
static bool init_valid(void)
{
    if (initialized)
        return true;
    for (unsigned a = 0; a < STATES; ++a)
    {
        for (unsigned b = 0; b < STATES; ++b)
        {
            for (unsigned c = 0; c < STATES; ++c)
            {
                bool ok = true;
                for (unsigned i = 0; i < WIDTH; ++i)
                {
                    if ((b >> i) & 1U)
                    {
                        /* Count orthogonal neighbours of this 1 */
                        unsigned vert = ((a >> i) & 1U) + ((c >> i) & 1U);
                        unsigned horiz =
                            (i > 0U   ? ((b >> (i - 1U)) & 1U) : 0U) +
                            (i < WIDTH-1U ? ((b >> (i + 1U)) & 1U) : 0U);
                        unsigned total = vert + horiz;
                        if (!(total == 1U || total == 3U || total == 4U))
                        {
                            ok = false;
                            break;
                        }
                    }
                }
                valid[a][b][c] = ok;
            }
        }
    }
    initialized = true;
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* Undefined for offset 1, but safe anyway */

    /* DP arrays for current and next row states */
    seq_t dp[STATES][STATES] = {0};
    seq_t dp_next[STATES][STATES] = {0};

    /* Ensure validity table is ready */
    init_valid();

    /* Initialise after first row: previous row is dummy 0 */
    for (unsigned curr = 0; curr < STATES; ++curr)
        dp[0][curr] = (seq_t)1;

    /* Process subsequent rows (k = 1 … n-1) */
    for (idx_t k = 1; k < n; ++k)
    {
        /* Clear next DP */
        for (unsigned i = 0; i < STATES; ++i)
            for (unsigned j = 0; j < STATES; ++j)
                dp_next[i][j] = (seq_t)0;

        for (unsigned prev = 0; prev < STATES; ++prev)
        {
            for (unsigned curr = 0; curr < STATES; ++curr)
            {
                seq_t val = dp[prev][curr];
                if (val == (seq_t)0)
                    continue;
                for (unsigned next = 0; next < STATES; ++next)
                {
                    if (valid[prev][curr][next])
                        dp_next[curr][next] += val;
                }
            }
        }

        /* Copy dp_next into dp for next iteration */
        for (unsigned i = 0; i < STATES; ++i)
            for (unsigned j = 0; j < STATES; ++j)
                dp[i][j] = dp_next[i][j];
    }

    /* Final row must have no neighbours below (next = 0) */
    seq_t result = (seq_t)0;
    for (unsigned prev = 0; prev < STATES; ++prev)
    {
        for (unsigned curr = 0; curr < STATES; ++curr)
        {
            if (dp[prev][curr] != (seq_t)0 && valid[prev][curr][0])
                result += dp[prev][curr];
        }
    }

    return result;
}
