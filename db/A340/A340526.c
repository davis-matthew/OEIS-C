/*
 * A340526  Triangle read by rows: T(n,k) = A006218(n-k+1)*A000041(k-1), 1 <=
 *          k <= n.
 * https://oeis.org/A340526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A340526
   Triangle read by rows:  T(n,k) = A006218(n-k+1) * A000041(k-1)   (1 <= k <= n)
   The sequence a(n) is obtained by reading the triangle row‑by‑row.
   a(1) = 1, a(2) = 3, a(3) = 1, a(4) = 5, a(5) = 3, a(6) = 2, …
   The values are computed from the convolution identity
      S[n] = sum_{i=1..n} A[i] * p[n-i]   with S[n] the cumulative
      total parts in partitions of all integers <= n.
   Partition numbers p[·] and total parts G[·] are built by a simple
   DP that simultaneously counts partitions and their parts.
   The implementation precomputes all required values up to a given
   number of rows and answers queries in O(1) time. */

#include <stddef.h> /* for size_t */

#define MAX_LIMIT 300      /* enough for n up to ~45k */
#define MAX_TOTAL ((MAX_LIMIT * (MAX_LIMIT + 1)) / 2)

/* The build supplies the integer types idx_t and seq_t. */
extern idx_t _floor_isqrt_u64(idx_t n); /* not used, but may be available */

/* Static storage for pre‑computed data */
static seq_t   p[MAX_LIMIT + 1];      /* partition numbers */
static seq_t   G[MAX_LIMIT + 1];      /* total parts in partitions of n */
static seq_t   S[MAX_LIMIT + 1];      /* cumulative G */
static seq_t   A[MAX_LIMIT + 1];      /* A006218 sequence */
static seq_t   a_seq[MAX_TOTAL + 1];  /* sequence a(n), 1‑based */
static idx_t   pre_limit = 0;         /* number of rows already computed */

/* Compute all tables up to 'limit' rows (1‑based). */
static void computeUpTo(idx_t limit)
{
    idx_t i, n, s;
    /* initialise arrays */
    for (i = 0; i <= limit; ++i) {
        p[i] = 0;
        G[i] = 0;
        S[i] = 0;
        A[i] = 0;
    }
    a_seq[0] = 0;               /* unused */
    /* base case */
    p[0] = 1;   /* p(0) = 1 */
    /* Partition DP: simultaneously count partitions and parts */
    for (s = 1; s <= limit; ++s) {
        for (n = s; n <= limit; ++n) {
            seq_t add_p = p[n - s];
            seq_t add_G = G[n - s];
            p[n] = p[n] + add_p;
            G[n] = G[n] + add_G + add_p;
        }
    }
    /* cumulative sums of total parts */
    S[0] = 0;
    for (n = 1; n <= limit; ++n)
        S[n] = S[n - 1] + G[n];
    /* Compute A006218 sequence from convolution identity */
    A[0] = 0;
    for (n = 1; n <= limit; ++n) {
        seq_t tmp = 0;
        for (i = 1; i < n; ++i)
            tmp = tmp + A[i] * p[n - i];
        A[n] = S[n] - tmp;
    }
    /* Build the triangle and its row‑by‑row reading */
    idx_t idx = 1;
    for (n = 1; n <= limit; ++n)
        for (i = 1; i <= n; ++i)
            a_seq[idx++] = A[n - i + 1] * p[i - 1];
    /* update precomputed limit */
    pre_limit = limit;
}

/* Return the nth term of A340526 (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    /* Determine number of rows needed to cover index n */
    idx_t rows = 0;
    while (rows * (rows + 1) / 2 < n)
        ++rows;
    if (rows > pre_limit)
        computeUpTo(rows);
    return a_seq[n];
}
