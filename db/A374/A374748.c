/*
 * A374748  Triangle read by rows where T(n,k) is the number of integer
 *          compositions of n whose leaders of weakly decreasing runs sum to
 *          k.
 * https://oeis.org/A374748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, c) where r is the row number and c the column inside
 *            the triangle. T(r, c) is the number of integer compositions of r
 *            whose leaders of weakly decreasing runs sum to c, as defined in
 *            OEIS A374748. The triangle is pre‑computed up to a constant size
 *            sufficient for all indices used (n ≤ 1...
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t, NULL    */
#include <stdbool.h>     /* bool           */
#include <stdlib.h>      /* malloc, memset */
#include <stdint.h>      /* for uint64_t    */
#include <math.h>        /* sqrtl for 64‑bit */

#define MAX_ROW 200     /* enough for all requested n (≈141)    */
static seq_t *dp = NULL;         /* 3‑D memoisation table              */
static bool   *vis = NULL;       /* visited flags for dp               */
static idx_t   dp_dim = 0;       /* dimension (MAX_ROW+1)              */

static inline size_t idx3(idx_t n, idx_t k, idx_t prev)
{
    /* Linear index in the flattened 3‑D array.
       dp_dim fits easily into size_t for the given limits.         */
    return ((size_t)n * dp_dim + k) * dp_dim + prev;
}

/* ------------------------------------------------------------------ */
/* Recursive computation with memoisation: ways(n, k, prev)              */
/* n   – remaining total sum                                         */
/* k   – remaining leaders sum                                       */
/* prev – maximal value allowed for the next part (last part of run)   */
/* ------------------------------------------------------------------ */
static seq_t ways(idx_t n, idx_t k, idx_t prev)
{
    if (n == 0)                /* no parts left */
        return (k == 0) ? 1 : 0;
    if (k > n)                 /* leaders cannot exceed sum */
        return 0;

    size_t id = idx3(n, k, prev);
    if (vis[id])
        return dp[id];
    vis[id] = true;

    seq_t res = 0;

    if (prev == 0) {           /* start of a new composition */
        for (idx_t p = 1; p <= n && p <= k; ++p)
            res += ways(n - p, k - p, p);
    } else {                   /* continue inside a run or start a new one */
        for (idx_t x = 1; x <= n; ++x) {
            if (x <= prev) {   /* stay in current run */
                res += ways(n - x, k, x);
            } else {           /* start a new run */
                if (x > k) break;          /* leaders sum would become negative */
                res += ways(n - x, k - x, x);
            }
        }
    }

    dp[id] = res;
    return res;
}

/* ------------------------------------------------------------------ */
/* Precomputation of the whole triangle up to MAX_ROW                 */
/* ------------------------------------------------------------------ */
static seq_t triangle[MAX_ROW + 1][MAX_ROW + 1];
static bool triangle_ready = false;

static void compute_triangle(void)
{
    if (triangle_ready)
        return;

    dp_dim = (size_t)(MAX_ROW + 1);
    size_t sz = dp_dim * dp_dim * dp_dim;

    dp  = (seq_t *)malloc(sz * sizeof(seq_t));
    vis = (bool  *)malloc(sz * sizeof(bool));
    if (!dp || !vis) {         /* allocation failure: abort gracefully */
        if (dp) free(dp);
        if (vis) free(vis);
        triangle_ready = false;
        return;
    }

    /* initialise visited flags */
    memset(vis, 0, sz * sizeof(bool));

    /* build the triangle */
    for (idx_t n = 0; n <= MAX_ROW; ++n) {
        for (idx_t k = 0; k <= n; ++k) {
            triangle[n][k] = ways(n, k, 0);
        }
    }

    triangle_ready = true;
}

/* ------------------------------------------------------------------ */
/* Helper to find row number r such that r(r+1)/2 <= n < (r+1)(r+2)/2  */
/* ------------------------------------------------------------------ */
static idx_t row_of(idx_t n)
{
    /* Integer square root, safe for n ≤ 1e4. Using sqrtl from <math.h>. */
    long double v = sqrtl((long double)8 * n + 1) - 1;
    return (idx_t)(v / 2);
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    compute_triangle();
    if (!triangle_ready)
        return 0;     /* fallback in case of allocation failure */

    idx_t r = row_of(n);                          /* row index   */
    idx_t start = r * (r + 1) / 2;                /* first index of row */
    idx_t c = n - start;                          /* column index */

    if (r > (idx_t)MAX_ROW || c > r)
        return 0;  /* out of pre‑computed bounds */

    return triangle[r][c];
}
