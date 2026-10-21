/*
 * A276468  Irregular triangular array: T(n,i) = number of partitions of n
 *          having crossover index k; see Comments.
 * https://oeis.org/A276468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *   OEIS A276468
 *
 *   T(n,i) = number of partitions of n having crossover index i.
 *   a(n) is the flat enumeration of the triangular array T(n,i)
 *   in row‑major order:  n = sum_{k=1}^{m-1} ceil(k/2) + i,
 *   where 1 ≤ i ≤ ceil(m/2).
 *
 *   The entry for larger n (up to ~10 000 in the tests) only needs rows up to
 *   250.  The program pre‑computes all partitions of n with largest part ≤ k
 *   (p(n,k)) and then evaluates T(n,i) by a simple summation that follows
 *   directly from the definition of the crossover index.
 */

#define NMAX 250            /* enough for the first ~15 000 terms          */

#include <stdbool.h>

/*  The table p[n][k] = number of partitions of n with largest part ≤ k   */
static seq_t p[NMAX + 1][NMAX + 1];
static bool dp_ready = false;

/*  The table T[n][i] = number of partitions of n with crossover index i */
static seq_t T[NMAX + 1][(NMAX + 2) / 2 + 1];
static bool T_ready = false;

/*------------------------------------------------------------*/
/*  Compute p[n][k]  (PD: partitions with largest part ≤ k)  */
/*------------------------------------------------------------*/
static void init_p(void)
{
    if (dp_ready) return;

    for (int k = 0; k <= NMAX; ++k) {
        for (int n = 0; n <= NMAX; ++n) {
            if (k > n) {
                p[n][k] = p[n][n];
            } else {
                if (n == 0) {
                    p[n][k] = (seq_t)1;          /* empty sum */
                } else if (k == 0) {
                    p[n][k] = (seq_t)0;          /* no parts allowed */
                } else {
                    p[n][k] = p[n][k - 1] + p[n - k][k];
                }
            }
        }
    }
    dp_ready = true;
}

/*------------------------------------------------------------*/
/*  Compute all T[n][i]  for 1 ≤ n ≤ NMAX                      */
/*------------------------------------------------------------*/
static void init_T(void)
{
    if (T_ready) return;
    init_p();

    for (int n = 1; n <= NMAX; ++n) {
        int H   = (n + 1) / 2;                 /* ceil(n/2) */
        int maxI = H;                          /* number of columns in row n */

        for (int i = 1; i <= maxI; ++i) {
            int j = i - 1;                     /* number of parts before the crossing part */
            seq_t sum = 0;

            for (int a = 1; a <= n; ++a) {    /* a = p_i  */
                int x_low  = j * a;
                int tmp = H - a;
                if (tmp > x_low) x_low = tmp; /* lower bound for X = prefix sum until i-1 parts */
                int x_high = H - 1;           /* upper bound for X */

                if (x_low > x_high) continue;

                for (int X = x_low; X <= x_high; ++X) {
                    int B = X - j * a;        /* sum of extra over j*a after subtraction of a from each part */
                    int R = n - X - a;        /* remaining sum of the partition */

                    /* ensure indices are within bounds of the table: they always are because
                       0 ≤ B ≤ n and 0 ≤ R ≤ n                                 */
                    seq_t prod = p[B][j] * p[R][a];
                    sum += prod;
                }
            }
            T[n][i] = sum;
        }
    }
    T_ready = true;
}

/*------------------------------------------------------------*/
/*  Main function required by the OEIS developer framework   */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* n is a 1‑based index in the flattened array                */
    init_T();

    idx_t cum = 0;                 /* number of entries before the current row */
    idx_t m = 1;                   /* current row number                     */

    for (;; ++m) {
        idx_t cols = (m + 1) / 2;   /* number of entries in this row         */
        if (cum + cols >= n) break;
        cum += cols;
    }

    if (m > NMAX) return (seq_t)0; /* out of pre‑computed range */

    idx_t i = n - cum;             /* 1‑based column index within row m     */
    return T[m][i];
}
