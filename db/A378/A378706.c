/*
 * A378706  Array read by ascending antidiagonals: A(n,k) is the total
 *          semiperimeter of n-Fibonacci polyominoes with k columns, where k >
 *          0.
 * https://oeis.org/A378706
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the array A(n,k), where A(n,k) is
 *            the total semiperimeter of n‑Fibonacci polyominoes with k
 *            columns, in ascending anti‑diagonals
 * Range:     u8 n <= 34, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers.  The type 'seq_t' is the integer type used by the
    build (uint8_t … uint64_t or an arbitrary‑precision integer class).
    The type 'idx_t' is an unsigned 64‑bit integer.  */
#include <stdint.h>
#include <stdlib.h>

/*  Maximum row and column that will be needed for any query with
    n ≤ 10000.  The index m = n‑2, the largest sum s needed satisfies
    (s‑2)(s‑1)/2 > m.  For m ≈ 10⁴ we have s < 150.  We use 200 to be
    safe.  */
#define MAX_ROW 200
#define MAX_K   200

/*  Storage for the precomputed values of A(n,k).  A[n][k] contains
    the value of A(n,k) for 2 ≤ n ≤ MAX_ROW and 1 ≤ k ≤ MAX_K.  */
static seq_t A_table[MAX_ROW + 1][MAX_K + 1];

/*  Flag to perform the one‑time precomputation.  */
static int precomputed = 0;

/*  Helper: compute the coefficients of A(n,k) for a fixed row n
    up to the given maximum index kmax (inclusive).  The result is
    stored in the buffer 'out', which must have size kmax+1.  */
static void compute_row(idx_t n, seq_t *out, size_t kmax)
{
    /*  Temporary integer arrays; all values fit in 64‑bit signed.  */
    int64_t *q = (int64_t*)malloc((kmax + 1) * sizeof(int64_t));
    int64_t *h = (int64_t*)malloc((kmax + 1) * sizeof(int64_t));
    int64_t *a = (int64_t*)malloc((kmax + 1) * sizeof(int64_t));
    int64_t *c = (int64_t*)malloc((kmax + 1) * sizeof(int64_t));
    int64_t *d = (int64_t*)malloc((kmax + 1) * sizeof(int64_t));
    int64_t *nnum = (int64_t*)malloc((kmax + 1) * sizeof(int64_t));
    int64_t *acoeff = (int64_t*)malloc((kmax + 1) * sizeof(int64_t));

    /*  Initialize to zero.  */
    for (size_t i = 0; i <= kmax; ++i) {
        q[i] = h[i] = a[i] = c[i] = d[i] = nnum[i] = acoeff[i] = 0;
    }

    /*  Q(y) = 1 - 4y + 4y² + 2yⁿ⁺¹ - 4yⁿ⁺² + y²ⁿ⁺²  */
    if (kmax >= 0)  q[0] = 1;
    if (kmax >= 1)  q[1] = -4;
    if (kmax >= 2)  q[2] = 4;
    if ((idx_t)(n + 1) <= kmax) q[n + 1] = 2;
    if ((idx_t)(n + 2) <= kmax) q[n + 2] = -4;
    if ((idx_t)(2 * n + 2) <= kmax) q[2 * n + 2] = 1;

    /*  H(y) = (1 - y) * Q(y)  →  h[i] = q[i] - q[i-1]  */
    h[0] = q[0];
    for (size_t i = 1; i <= kmax; ++i) {
        h[i] = q[i] - q[i - 1];
    }

    /*  a_i : coefficients of 1 - 2y - 2yⁿ + 3yⁿ⁺¹  */
    a[0] = 1;
    a[1] = -2;
    if ((idx_t)n <= kmax)         a[n]     = -2;
    if ((idx_t)n + 1 <= kmax)     a[n + 1] = 3;

    /*  c_i : coefficients of -1 + y - y² + yⁿ⁺²  */
    c[0] = -1;
    c[1] = 1;
    c[2] = -1;
    if ((idx_t)n + 2 <= kmax) c[n + 2] = 1;

    /*  d_i : coefficients of (1 - yⁿ) * c_i  = c_i - c_{i-n}  */
    for (size_t i = 0; i <= kmax; ++i) {
        d[i] = c[i];
        if (i >= (size_t)n) {
            d[i] -= c[i - n];
        }
    }

    /*  N_k  : coefficients of the numerator  */
    for (size_t k = 0; k <= kmax; ++k) {
        int64_t term1 = 0;
        int64_t akm1 = (k >= 1) ? a[k - 1] : 0;
        int64_t akm2 = (k >= 2) ? a[k - 2] : 0;
        term1 = (int64_t)n * (akm1 - akm2);

        int64_t term2 = 0;
        if (k >= 1) {
            term2 = d[k - 1];
        }
        nnum[k] = term1 - term2;
    }

    /*  Recurrence to get A_k : A_k = N_k - Σ_{i=1}^{k} h_i * A_{k-i}  */
    acoeff[0] = nnum[0];
    for (size_t k = 1; k <= kmax; ++k) {
        __int128 sum = 0;
        for (size_t i = 1; i <= k; ++i) {
            sum += (__int128)h[i] * acoeff[k - i];
        }
        acoeff[k] = (int64_t)(nnum[k] - sum);
    }

    /*  Store the result into the output array (sequence starts at 1).  */
    for (size_t k = 1; k <= kmax; ++k) {
        out[k] = (seq_t)acoeff[k];
    }

    /*  Free temporaries.  */
    free(q);
    free(h);
    free(a);
    free(c);
    free(d);
    free(nnum);
    free(acoeff);
}

/*  Precompute all rows for the required range.  */
static void precompute(void)
{
    if (precomputed) return;
    /*  Each row is computed up to MAX_K.  */
    for (idx_t n = 2; n <= MAX_ROW; ++n) {
        compute_row(n, &A_table[n][0], MAX_K);
    }
    precomputed = 1;
}

/*  Main function requested by the test harness.  */
seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /*  Convert to zero‑based index inside the sequence.  */
    idx_t m = n - 2;               /* position of term in the anti‑diagonal order */

    /*  Find the smallest sum s such that (s-2)(s-1)/2 > m.  */
    idx_t s = 3;
    while ((idx_t)((s - 2) * (s - 1) / 2) <= m) ++s;

    idx_t prev = (idx_t)((s - 3) * (s - 2) / 2);   /* number of terms before this diagonal */
    idx_t offset = m - prev;                        /* zero‑based index inside diagonal */

    idx_t row = s - 1 - offset;     /* n (row index in the array)  */
    idx_t col = offset + 1;         /* k (column index in the array) */

    precompute();                    /* ensure all needed rows are ready */

    return A_table[row][col];
}
