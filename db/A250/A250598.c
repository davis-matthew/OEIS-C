/*
 * A250598  Number of (n+2)X(5+2) 0..2 arrays with nondecreasing sum of every
 *          three consecutive values in every row and column.
 * https://oeis.org/A250598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product over residues i,j of F(rows(i), cols(j)) where
 *            rows(i) = ((n+1 - i)/3)+1, cols(0)=3, cols(1)=2, cols(2)=2 and
 *            F(r,c) is the number of nondecreasing r×c matrices with entries
 *            0,1,2. The array is counted by dividing the grid into 3×3
 *            independent subgrids (induced by i mod 3 and j mod...
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 9, u128 n <= 20,
 *            big any n
 * Helpers:   generate_sequences
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/generate_sequences.h"

/* Forward declaration of seq_t, idx_t supplied by build. */
typedef idx_t              idx_t;   /* integer type for indices */
typedef seq_t              seq_t;   /* integer type for sequence values */

/* Maximum number of rows per residue for n <= 10000. */
#define MAX_R 3335

/* Precomputed counts F(r,c) for c=1,2,3 and r up to MAX_R. */
static seq_t F[4][MAX_R + 1];        /* index by c (1..3) */

/* Helper: compute DP tables for a given column size c. */
static void precompute_F_for_c(int c)
{
    int seqs[30];     /* at most 10 sequences for c=3, fits in 30 */
    int m, idx = 0;
    generate_sequences(c, seqs, &m);

    /* less[i][j] true if sequence i <= sequence j pointwise */
    bool less[30][30];
    for (int i = 0; i < m; ++i)
    {
        int a = seqs[i];
        int digits_i[3];
        int tmp = a;
        for (int k = 0; k < c; ++k) { digits_i[k] = tmp % 3; tmp /= 3; }

        for (int j = 0; j < m; ++j)
        {
            int b = seqs[j];
            int digits_j[3];
            int tmpj = b;
            bool le = true;
            for (int k = 0; k < c; ++k)
            {
                int d = tmpj % 3;
                tmpj /= 3;
                if (digits_i[k] > d) { le = false; break; }
            }
            less[i][j] = le;
        }
    }

    /* DP: dp_prev[i] = number of chains of current length ending at i */
    seq_t dp_prev[30], dp_curr[30];
    for (int i = 0; i < m; ++i) dp_prev[i] = 1;  /* length 1 chains */
    F[c][1] = m;

    for (int len = 2; len <= MAX_R; ++len)
    {
        seq_t sum = 0;
        for (int j = 0; j < m; ++j)
        {
            seq_t s = 0;
            for (int i = 0; i < m; ++i)
                if (less[i][j]) s += dp_prev[i];
            dp_curr[j] = s;
            sum += s;
        }
        F[c][len] = sum;
        for (int i = 0; i < m; ++i) dp_prev[i] = dp_curr[i];
    }
}

/* One-time global initialization of F tables. */
static bool F_inited = false;
static void init_F(void)
{
    if (F_inited) return;
    for (int c = 1; c <= 3; ++c) precompute_F_for_c(c);
    F_inited = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but return 0 */

    init_F();

    /* compute rows per residue i=0,1,2 */
    idx_t rows[3];
    idx_t total_rows = n + 2;          /* n+2 rows indexed 0..n+1 */
    for (int r = 0; r < 3; ++r)
    {
        if (r > n + 1) rows[r] = 0;
        else rows[r] = ((n + 1 - r) / 3) + 1;
    }

    /* columns per residue j=0,1,2: values 3,2,2 */
    idx_t cols[3] = {3, 2, 2};

    seq_t result = 1;
    for (int i = 0; i < 3; ++i)
    {
        idx_t r = rows[i];
        for (int j = 0; j < 3; ++j)
        {
            idx_t c = cols[j];
            result *= F[c][r];
        }
    }
    return result;
}
