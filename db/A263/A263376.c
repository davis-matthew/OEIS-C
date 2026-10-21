/*
 * A263376  Number of (n+1)X(2+1) 0..4 arrays with each row and column
 *          divisible by 9, read as a base-5 number with top and left being
 *          the most significant digits.
 * https://oeis.org/A263376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 0..4 arrays with each * row and column
 *            divisible by 9, read as a base‑5 number * (top row most
 *            significant, left column most significant)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses dynamic programming over column
 * residues modulo 9.  For each row we consider only
 * the 14 triples (x0,x1,x2) that satisfy the row
 * divisibility condition 7·x0 + 5·x1 + x2 ≡ 0 (mod 9).
 * The residue of a column after k rows is
 *
 *   r_j = ( Σ_{i=0}^{k} 5^{i}·x_{i,j} ) mod 9
 *
 * Processing rows from bottom to top gives a simple
 * periodic weight pattern w = 5^{i} mod 9 with period 6.
 * The DP state is the triple (r0,r1,r2).  There are
 * 9³=729 states, and for each we pre‑compute the
 * transition to the next state for each of the 6
 * possible weights and each of the 14 valid triples.
 * The DP runs in O(6·729·14·n) ≈ 1.6·10⁸ elementary
 * operations for n=10 000, well within the limit. */

#include <stdint.h>
#include <stddef.h>

static const int pow5_mod[6] = {1, 5, 7, 8, 4, 2};

/* The 14 valid row triples (x0,x1,x2) such that
 * 7·x0 + 5·x1 + x2 ≡ 0 (mod 9), 0 ≤ xi ≤ 4. */
static const unsigned char rowTriples[14][3] = {
    {0,0,0}, {0,1,4}, {0,3,3},
    {1,0,2}, {1,2,1}, {1,4,0},
    {2,0,4}, {2,2,3}, {2,4,2},
    {3,1,1}, {3,3,0}, {3,4,4},
    {4,1,3}, {4,3,2}
};

static const unsigned char numTriples = 14;

/* Precomputed transition table.
 * trans[w][state][t] gives the index of the new state
 * after applying triple t (0..13) when the current weight
 * is pow5_mod[w] (w = 0..5).  State index is
 *  res0*81 + res1*9 + res2  (0 ≤ res0,1,2 < 9). */
static unsigned short trans[6][729][14];

/* Table lazily initialized on first call to a(). */
static int trans_initialized = 0;

static void init_transitions(void)
{
    if (trans_initialized) return;
    for (int w = 0; w < 6; ++w) {
        int weight = pow5_mod[w];
        for (int res0 = 0; res0 < 9; ++res0) {
            for (int res1 = 0; res1 < 9; ++res1) {
                for (int res2 = 0; res2 < 9; ++res2) {
                    int s = res0 * 81 + res1 * 9 + res2;
                    for (int t = 0; t < numTriples; ++t) {
                        int a = rowTriples[t][0];
                        int b = rowTriples[t][1];
                        int c = rowTriples[t][2];
                        int nr0 = (res0 + weight * a) % 9;
                        int nr1 = (res1 + weight * b) % 9;
                        int nr2 = (res2 + weight * c) % 9;
                        int ns = nr0 * 81 + nr1 * 9 + nr2;
                        trans[w][s][t] = (unsigned short)ns;
                    }
                }
            }
        }
    }
    trans_initialized = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n>=1 as per offset 1, but handle n==0 gracefully. */
    if (n == 0) return (seq_t)0;

    init_transitions();

    /* Allocate two DP arrays: current and next */
    size_t sz = 729;
    seq_t *dp_cur = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp_cur || !dp_next) { /* allocation failed */
        if (dp_cur) free(dp_cur);
        if (dp_next) free(dp_next);
        return (seq_t)0;
    }

    dp_cur[0] = (seq_t)1;        /* start with all residues zero */
    for (idx_t step = 0; step <= n; ++step) {   /* process n+1 rows */
        int w = step % 6;        /* weight index for this row */
        /* zero next array */
        for (size_t i = 0; i < sz; ++i)
            dp_next[i] = (seq_t)0;
        /* transition */
        for (size_t s = 0; s < sz; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            for (int t = 0; t < numTriples; ++t) {
                size_t ns = trans[w][s][t];
                dp_next[ns] += val;
            }
        }
        /* swap */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    seq_t result = dp_cur[0];   /* residues all zero */
    free(dp_cur);
    free(dp_next);
    return result;
}
