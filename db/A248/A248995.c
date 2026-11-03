/*
 * A248995  Number of length n+4 0..2 arrays with no five consecutive terms
 *          having two times the sum of any three elements equal to three
 *          times the sum of the remaining two.
 * https://oeis.org/A248995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 19, u64 n <= 44, u128 n <= 94,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 202 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute OEIS sequence A248995
 * a(n) = number of length n+4 arrays over {0,1,2}
 *        with no five consecutive terms in which
 *        2*(sum of any 3 terms) == 3*(sum of the remaining 2 terms).
 *
 * The implementation uses a finite‑state automaton.
 * There are 3⁴ = 81 states, each representing the last 4
 * elements of the partially constructed array.
 * For every state and next symbol 0..2 we precompute whether
 * the new 5‑tuple is allowed and, if so, the resulting state.
 * A simple DP runs in O(n) time and O(81) memory.
 */

#include <stdint.h>
#include <stdbool.h>

/* Encode a quadruple (a,b,c,d) of digits 0..2 into a state 0..80 */
static inline int state_enc(int a, int b, int c, int d)
{
    return ((a * 3 + b) * 3 + c) * 3 + d;
}

/* Check whether a 5‑tuple satisfies the forbidden condition. */
static bool tuple_good(int a, int b, int c, int d, int e)
{
    int s[5] = { a, b, c, d, e };
    int total = a + b + c + d + e;
    /* Enumerate all 10 combinations of 3 indices */
    for (int i = 0; i < 5; ++i)
        for (int j = i + 1; j < 5; ++j)
            for (int k = j + 1; k < 5; ++k)
            {
                int sum_sel = s[i] + s[j] + s[k];
                int sum_oth = total - sum_sel;
                if (2 * sum_sel == 3 * sum_oth)
                    return false;  /* forbidden */
            }
    return true; /* good */
}

static bool good5[243];                     /* 3⁵ */
static int  trans[81][3];                   /* transition table, -1 = forbidden */
static bool initialized = false;

/* Simple initialization of tables, executed once. */
static void init_tables(void)
{
    if (initialized) return;
    initialized = true;

    /* Precompute goodness of all 243 5‑tuples */
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                for (int d = 0; d < 3; ++d)
                    for (int e = 0; e < 3; ++e)
                    {
                        int id = ((a * 3 + b) * 3 + c) * 9 + d * 3 + e; /* 3⁴*3 index  */
                        good5[id] = tuple_good(a, b, c, d, e);
                    }

    /* Build transition table */
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                for (int d = 0; d < 3; ++d)
                {
                    int state = state_enc(a, b, c, d);
                    for (int e = 0; e < 3; ++e)
                    {
                        int idx = ((a * 3 + b) * 3 + c) * 9 + d * 3 + e;
                        if (good5[idx])
                        {
                            int next_state = ((b * 3 + c) * 3 + d) * 3 + e;
                            trans[state][e] = next_state;
                        }
                        else
                            trans[state][e] = -1;
                    }
                }
}

/* ------------------------------------------------------------------ */
/* Main function required by the outline.                            */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* below offset; behaviour irrelevant */

    init_tables();

    const idx_t len = n + 4;      /* total array length */

    /* DP arrays: current and next counts for each state */
    seq_t curr[81];
    seq_t nxt [81];

    /* Initialize for length 4 (any 4‑tuple is allowed) */
    for (int s = 0; s < 81; ++s)
        curr[s] = 1;

    /* Append elements until total length len is reached */
    for (idx_t pos = 5; pos <= len; ++pos)
    {
        /* zero next counts */
        for (int s = 0; s < 81; ++s)
            nxt[s] = 0;

        for (int s = 0; s < 81; ++s)
            if (curr[s] != 0) {
                seq_t val = curr[s];
                for (int v = 0; v < 3; ++v) {
                    int ns = trans[s][v];
                    if (ns != -1) nxt[ns] += val;
                }
            }

        /* move nxt to curr for the next iteration */
        for (int s = 0; s < 81; ++s) curr[s] = nxt[s];
    }

    /* Sum over all states to get the total count */
    seq_t total = 0;
    for (int s = 0; s < 81; ++s) total += curr[s];
    return total;
}
