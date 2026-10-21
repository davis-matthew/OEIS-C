/*
 * A277517  Irregular triangle read by rows: T(n,k) is the maximum number of
 *          common subsequences of k distinct permutations of n items, with
 *          n>=1 and 1<=k<=n!.
 * https://oeis.org/A277517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   A277517 is the flattened irregular triangle defined by T(1,1)=2,
 *            T(2,1)=4, T(2,2)=3, and for n>2 T(n,k)= 2*T(n-1,k) for 1 ≤ k ≤
 *            (n-1)! 2*T(n-1,(n-1)!)-(k-(n-1)!) for (n-1)!+1 ≤ k ≤ (n-1)!+n-2
 *            n+1 for (n-1)!+n-1 ≤ k ≤ n! . The sequence a(n) is obtained by
 *            reading the triangle row by row
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_ROW 12                       /* enough for all indices up to 10000 */

static idx_t fact[MAX_ROW + 1];          /* factorial table */
static idx_t cum[MAX_ROW + 1];           /* cumulative sums of factorials */
static int  init_done = 0;               /* initialization flag */

/* Recursive computation of T(n,k) with the recurrence above */
static seq_t T_rec(idx_t n, idx_t k)
{
    if (n == 1) {
        return (seq_t)2;
    }
    if (n == 2) {
        return (k == 1) ? (seq_t)4 : (seq_t)3;
    }
    idx_t fprev = fact[n-1];
    if (k <= fprev) {
        return (seq_t)2 * T_rec(n-1, k);
    } else if (k <= fprev + n - 2) {
        seq_t tprev = T_rec(n-1, fprev);
        return (seq_t)2 * tprev - (seq_t)(k - fprev);
    } else {
        return (seq_t)(n + 1);
    }
}

/* Public function required by the tests */
seq_t a(idx_t n)
{
    /* One-time initialization of factorials and cumulative sums */
    if (!init_done) {
        fact[0] = 1;
        cum[0]  = 0;
        for (int i = 1; i <= MAX_ROW; ++i) {
            fact[i] = fact[i-1] * (idx_t)i;
            cum[i]  = cum[i-1] + fact[i];
        }
        init_done = 1;
    }

    /* Find the row where the index n lies (1‑based) */
    int row = 1;
    while (row <= MAX_ROW && cum[row] < n) {
        ++row;
    }

    /* Position within that row */
    idx_t k = n - cum[row-1];

    /* Compute the value using the recurrence */
    return T_rec((idx_t)row, k);
}
