/*
 * A357868  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..n} (k*j)!* Stirling2(n,k*j).
 * https://oeis.org/A357868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(0)=1, a(1)=1, a(2)=0,... defined by A357868. The square array
 *            T(n,k) (n>=0,k>=0) is entered as T(n,k) = Sum_{j=0..n} (k*j)! *
 *            Stirling2(n,k*j) It satisfies T(0,k)=1 and for k>0 T(n,k) = k! *
 *            Sum_{j=1..n} C(n,j) * Stirling2(j,k) * T(n-j,k). The sequence
 *            a(n) is obtained by reading the array on a...
 * Range:     u8 n <= 25, u16 n <= 52, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binom.h"

/* Static tables, filled on first call.  MAXN guarantees all needed
   values for n up to 10000 (max s ≈ 140). */
#define MAXN 200

static seq_t *fac = NULL;          /* factorial[0..MAXN]   */
static seq_t **S = NULL;           /* Stirling2[n][k]      */
static seq_t **T = NULL;           /* T[n][k]              */
static int tables_initialized = 0;

/* Forward declaration of helper to initialise tables. */
static void init_tables(void);

/* -------------------------------------------------------------------- */
static void init_tables(void)
{
    if (tables_initialized) return;
    /* Allocate factorial array. */
    fac = (seq_t *)malloc((MAXN + 1) * sizeof(seq_t));
    fac[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAXN; ++i)
        fac[i] = fac[i - 1] * (seq_t)i;

    /* Allocate 2‑D arrays for Stirling numbers (rectangular for simplicity). */
    S = (seq_t **)malloc((MAXN + 1) * sizeof(seq_t *));
    for (idx_t i = 0; i <= MAXN; ++i)
        S[i] = (seq_t *)calloc((MAXN + 1), sizeof(seq_t));

    /* Stirling recurrence: S(0,0)=1, S(n,0)=0 for n>0. */
    S[0][0] = (seq_t)1;
    for (idx_t n = 1; n <= MAXN; ++n)
    {
        S[n][0] = (seq_t)0;
        for (idx_t k = 1; k <= n; ++k)
        {
            /* S(n,k) = k*S(n-1,k) + S(n-1,k-1) */
            S[n][k] = S[n - 1][k] * (seq_t)k + S[n - 1][k - 1];
        }
    }

    /* Allocate 2‑D array for T. Again rectangular. */
    T = (seq_t **)malloc((MAXN + 1) * sizeof(seq_t *));
    for (idx_t n = 0; n <= MAXN; ++n)
        T[n] = (seq_t *)calloc((MAXN + 1), sizeof(seq_t));

    /* Base row: T[0][k] = 1 for all k. */
    for (idx_t k = 0; k <= MAXN; ++k)
        T[0][k] = (seq_t)1;

    /* Compute T[n][k] for n>=1.   For k=0, T[n][0] = (n==0?1:0). */
    for (idx_t n = 1; n <= MAXN; ++n)
    {
        T[n][0] = (seq_t)0; /* n>0 */
        for (idx_t k = 1; k <= MAXN; ++k)
        {
            seq_t sum = (seq_t)0;
            for (idx_t j = k; j <= n; ++j)
            {
                /* term = C(n,j) * Stirling2(j,k) * T[n-j][k] */
                seq_t term = binom((idx_t)n, (idx_t)j) * S[j][k] * T[n - j][k];
                sum += term;
            }
            T[n][k] = fac[k] * sum;
        }
    }

    tables_initialized = 1;
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise tables on first invocation. */
    init_tables();

    /* Find the antidiagonal index s such that (s+1)(s+2)/2 > n. */
    idx_t s = 0;
    idx_t cumulative;
    do
    {
        cumulative = (s + 1) * (s + 2) / 2; /* cumulative terms up to diagonal s */
        if (cumulative > n)
            break;
        ++s;
    } while (1);

    /* Position p along diagonal (0 <= p <= s).  The first element of the
       diagonal has p=0 (row 0, column s).  Indexing: p = n - previous_cumulative */
    idx_t prev_cumulative = s * (s + 1) / 2;  /* cumulative up to s-1 */
    idx_t p = n - prev_cumulative;           /* row index in the array */

    /* Coordinates: i = p (row), j = s - p (column) */
    idx_t i = p;
    idx_t j = s - p;

    return T[i][j];
}
