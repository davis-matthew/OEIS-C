/*
 * A321331  Triangle read by rows: T(n, k) = (k+1)*S2(n+1, k+1), for n >= k >=
 *          0, and S2 = A048993 (Stirling2).
 * https://oeis.org/A321331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r(k),k) where * r is the row such that r*(r+1)/2 <= n <
 *            (r+1)*(r+2)/2 * k = n - r*(r+1)/2 * T(r,k) = (k+1) * S2(r+1,k+1)
 *            * with S2(n,k) the Stirling numbers of the second kind *
 *            (recurrence: S2(0,0)=1, S2(n,0)=0 (n>0), S2(n,n)=1, *
 *            S2(n,k)=k*S2(n-1,k)+S2(n-1,k-1))
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Upper bound on required Stirling degrees for n up to 10000  
 * (the largest r is 140, so 200 is safe). */
#define MAX_STIR_N 200

static bool init_done = false;
static seq_t S2[MAX_STIR_N + 1][MAX_STIR_N + 1];

static void init_stirling(void)
{
    S2[0][0] = (seq_t)1;
    for (size_t n = 1; n <= MAX_STIR_N; ++n) {
        S2[n][0] = (seq_t)0;
        for (size_t k = 1; k <= n; ++k) {
            if (k == n) {
                S2[n][k] = (seq_t)1;
            } else {
                S2[n][k] = (seq_t)k * S2[n - 1][k] + S2[n - 1][k - 1];
            }
        }
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    if (!init_done)
        init_stirling();

    /* Find the row r via cumulative counts */
    idx_t pos   = 0;                 /* number of elements already consumed   */
    size_t r    = 0;                 /* current row index (0‑based)            */
    while (pos + (idx_t)(r + 1) <= n) {
        pos += (idx_t)(r + 1);
        ++r;
    }

    /* r is now the correct row, and pos is the starting index of that row */
    size_t k      = (size_t)(n - pos);          /* column index within the row   */
    size_t row_n  = r + 1;                      /* argument for Stirling numbers */

    seq_t val = S2[row_n][k + 1];
    return (seq_t)(k + 1) * val;
}
