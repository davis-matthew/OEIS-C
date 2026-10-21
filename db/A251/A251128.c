/*
 * A251128  T(n,k)=Number of (n+1)X(k+1) 0..1 arrays with no 2X2 subblock
 *          having the sum of its diagonal elements greater than the maximum
 *          of its antidiagonal elements.
 * https://oeis.org/A251128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let T(n,k) be the number of (n+1)x(k+1) binary arrays with the
 *            property that no 2x2 subblock has the sum of its diagonal
 *            elements greater than the maximum of its anti‑diagonal elements.
 *            For fixed k the column sequence a_k(n)=T(n,k) satisfies if k==1
 *            a_k(n)=3*a_k(n‑1)-2*a_k(n‑2)-a_k(n‑3)+a_k(n‑4)...
 * Range:     u8 n <= 21, u16 n <= 153, u32 n <= 820, u64 n <= 1104,
 *            u128 n <= 1104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

#define MAX_N 500   /* sufficient for n up to about 2·10^5 and beyond */

static bool ready = false;
static seq_t table[MAX_N+1][MAX_N+1];

/* pre‑compute T(n,k) for 1 ≤ n ≤ 6  and 1 ≤ k ≤ MAX_N  using DP,
   then extend to larger n by the recurrences.  */
static void init(void)
{
    /* DP for fixed n (rows = n+1) */
    for (int n = 1; n <= 6; ++n) {
        int rows = n + 1;
        int S = 1 << rows;                     /* number of column states */
        bool allowed[128][128] = {false};       /* S ≤ 128 */

        /* build allowed transitions */
        for (int s = 0; s < S; ++s) {
            for (int t = 0; t < S; ++t) {
                bool ok = true;
                for (int i = 0; i < n; ++i) {
                    int a_i   = (s >> i)     & 1;
                    int a_ip1 = (s >> (i+1)) & 1;
                    int b_i   = (t >> i)     & 1;
                    int b_ip1 = (t >> (i+1)) & 1;
                    int diag = a_i + b_ip1;
                    int anti = (b_i > a_ip1) ? b_i : a_ip1;
                    if (diag > anti) { ok = false; break; }
                }
                allowed[s][t] = ok;
            }
        }

        /* count arrays of width k+1 columns by DP over column states */
        seq_t dp_prev[128], dp_next[128];
        for (int s = 0; s < S; ++s) dp_prev[s] = 1;

        for (int k = 1; k <= MAX_N; ++k) {
            for (int t = 0; t < S; ++t) {
                seq_t sum = 0;
                for (int s = 0; s < S; ++s)
                    if (allowed[s][t]) sum += dp_prev[s];
                dp_next[t] = sum;
            }
            seq_t total = 0;
            for (int t = 0; t < S; ++t) total += dp_next[t];
            table[n][k] = total;
            for (int t = 0; t < S; ++t) dp_prev[t] = dp_next[t];
        }
    }

    /* extend tables by the recurrences for all visible columns */
    for (int k = 1; k <= MAX_N; ++k) {
        /* k == 1 : order‑4 recurrence */
        for (int i = 7; i <= MAX_N; ++i) {
            table[i][k] = (seq_t)3 * table[i-1][k]
                          - (seq_t)2 * table[i-2][k]
                          -        table[i-3][k]
                          +        table[i-4][k];
        }
        /* k >= 2 : order‑5 recurrence */
        if (k >= 2) {
            for (int i = 7; i <= MAX_N; ++i) {
                table[i][k] = (seq_t)4 * table[i-1][k]
                              - (seq_t)5 * table[i-2][k]
                              +        table[i-3][k]
                              + (seq_t)2 * table[i-4][k]
                              -        table[i-5][k];
            }
        }
    }
    ready = true;
}

seq_t a(idx_t n)
{
    if (!ready) init();

    /* find diagonal d such that d*(d-1)/2 >= n (> =) */
    idx_t d = 2;
    while ((d * (d - 1)) / 2 < n) ++d;

    idx_t prevC = (d - 1) * (d - 2) / 2;   /* C(d-1) */
    idx_t row   = n - prevC;                /* 1 ≤ row ≤ d-1 */
    idx_t col   = d - row;                  /* 1 ≤ col ≤ d-1 */

    return table[(int)row][(int)col];
}
