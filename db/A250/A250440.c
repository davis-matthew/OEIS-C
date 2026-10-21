/*
 * A250440  Number of (n+1)X(5+1) 0..2 arrays with nondecreasing sum of every
 *          two consecutive values in every row and column.
 * https://oeis.org/A250440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)x6 arrays filled with 0,1,2 such that the
 *            sum of every two consecutive cells in each row and each column
 *            is nondecreasing. The computation is performed by counting
 *            chains in the partially ordered set of all valid 6‑tuples
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 23, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* maximum n allowed by the problem statement */
#define MAX_N 100
#define MAX_T 5002   /* maximum chain length needed */

/* static tables holding all valid row patterns (100 patterns) */
static unsigned char rows[MAX_N][6];
static int preds[MAX_N][MAX_N];
static unsigned int predcnt[MAX_N];
static unsigned int N = 0;

/* dynamic programming arrays */
static seq_t dp_curr[MAX_N];
static seq_t dp_next[MAX_N];
static seq_t totals[MAX_T];
static unsigned int cur_len = 1;

/* flag indicating whether tables have been built */
static int initialized = 0;

/* --------------------------------------------------------------- */
/* Build the set of all 6‑tuples with 0<=a_i<=2 satisfying
   a0<=a2<=a4 and a1<=a3<=a5.  Compute the poset relations.   */
static void build_tables(void)
{
    unsigned int mask;
    unsigned int count = 0;

    /* enumerate all 3^6 tuples */
    for (mask = 0; mask < 729; ++mask) {
        unsigned int t = mask;
        unsigned char v[6];
        int ok = 1;
        for (int i = 0; i < 6; ++i) {
            v[i] = t % 3;
            t /= 3;
        }
        /* horizontal monotone sums => a[i] <= a[i+2] for i=0..3 */
        if (!(v[0] <= v[2] && v[2] <= v[4] &&
              v[1] <= v[3] && v[3] <= v[5]))
            ok = 0;
        if (ok) {
            for (int i = 0; i < 6; ++i)
                rows[count][i] = v[i];
            ++count;
        }
    }
    N = count;   /* should be 100 */

    /* build the partial order: rows[i] <= rows[j] componentwise */
    for (unsigned int j = 0; j < N; ++j) {
        unsigned int pc = 0;
        for (unsigned int i = 0; i < N; ++i) {
            int le = 1;
            for (int c = 0; c < 6; ++c)
                if (rows[i][c] > rows[j][c]) { le = 0; break; }
            if (le)
                preds[j][pc++] = (int)i;
        }
        predcnt[j] = pc;
    }

    /* initialise DP for chain length 1 */
    for (unsigned int i = 0; i < N; ++i)
        dp_curr[i] = (seq_t)1;

    totals[1] = (seq_t)N;
    cur_len = 1;
    initialized = 1;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized)
        build_tables();

    /* number of rows = n+1 */
    idx_t rows_cnt = n + (idx_t)1;

    /* number of odd and even rows */
    idx_t k = (rows_cnt + (idx_t)1) / (idx_t)2;   /* ceil((rows_cnt)/2) */
    idx_t l = rows_cnt / (idx_t)2;                /* floor((rows_cnt)/2) */

    /* enlarge DP tables if necessary */
    idx_t need = (k > l) ? k : l;
    while (cur_len < need) {
        /* compute chains of next length */
        for (unsigned int j = 0; j < N; ++j)
            dp_next[j] = (seq_t)0;

        for (unsigned int j = 0; j < N; ++j) {
            seq_t sum = (seq_t)0;
            unsigned int pc = predcnt[j];
            int *p = preds[j];
            for (unsigned int t = 0; t < pc; ++t)
                sum += dp_curr[p[t]];
            dp_next[j] = sum;
        }

        /* store total number of chains */
        seq_t total = (seq_t)0;
        for (unsigned int j = 0; j < N; ++j)
            total += dp_next[j];

        ++cur_len;
        totals[cur_len] = total;

        /* advance dp arrays */
        for (unsigned int j = 0; j < N; ++j)
            dp_curr[j] = dp_next[j];
    }

    /* answer is product of counts for odd and even rows */
    seq_t ans = totals[(size_t)k] * totals[(size_t)l];
    return ans;
}
