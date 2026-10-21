/*
 * A269940  Triangle read by rows, T(n, k) = Sum_{m=0..k} (-1)^(m +
 *          k)*binomial(n + k, n + m) * |Stirling1(n + m, m)|, for n >= 0, 0
 *          <= k <= n.
 * https://oeis.org/A269940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   T(n,k) = ∑_{m=0..k} (-1)^{m+k} * binomial(n+k, n+m) *
 *            |Stirling1(n+m,m)| a(n) = T(row,col) where n = row*(row+1)/2 +
 *            col (row ≥ 0, 0 ≤ col ≤ row)
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- unsigned Stirling numbers of the first kind (cycle numbers) ---------- */
static seq_t *stirling = NULL;   /* linear storage: trot[i * cap + j] = s(i,j) */
static idx_t  stirling_cap = 0;  /* current capacity of the square table */

/* Grow the Stirling table so that it can hold values up to index N (inclusive) */
static void ensure_stirling(idx_t N)
{
    if (N < stirling_cap) return;

    /* allocate new capacity (power of two, at least N+1) */
    idx_t newcap = stirling_cap ? stirling_cap : 1;
    while (newcap <= N) newcap <<= 1;

    /* allocate new array */
    seq_t *newptr = (seq_t *)calloc((size_t)newcap * (size_t)newcap, sizeof(seq_t));
    if (!newptr) abort();

    /* copy existing values */
    for (idx_t i = 0; i < stirling_cap; ++i)
        for (idx_t j = 0; j <= i; ++j)
            newptr[i * newcap + j] = stirling[i * stirling_cap + j];

    /* free old array */
    free(stirling);
    stirling = newptr;

    /* recompute all values up to newcap-1 (small, max ~150) */
    for (idx_t i = 0; i < newcap; ++i)
        for (idx_t j = 0; j <= i; ++j)
        {
            if (i == 0 && j == 0) { stirling[0] = (seq_t)1; continue; }
            if (j == 0 || j > i)   { stirling[i * newcap + j] = (seq_t)0; continue; }
            seq_t a = stirling[(i-1) * newcap + (j-1)];
            seq_t b = stirling[(i-1) * newcap + j] * (seq_t)(i-1);
            stirling[i * newcap + j] = a + b;
        }

    stirling_cap = newcap;
}

/* Return unsigned Stirling1(i,j) (absolute value) */
static seq_t dstirling(idx_t i, idx_t j)
{
    ensure_stirling(i);
    return stirling[i * stirling_cap + j];
}

/* ---------- binomial coefficient (unsigned) ---------- */
static seq_t binom(idx_t a, idx_t b)
{
    if (b > a) return (seq_t)0;
    if (b > a - b) b = a - b;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= b; ++i)
    {
        /* multiply then divide: sequentially preserves integer arithmetic */
        res = (res * (seq_t)(a - b + i)) / (seq_t)i;
    }
    return res;
}

/* ---------- compute T(n,k) ---------- */
static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t result = (seq_t)0;
    for (idx_t m = 0; m <= k; ++m)
    {
        seq_t absS = dstirling(n + m, m);
        seq_t bin  = binom(n + k, n + m);
        seq_t term = bin * absS;
        if ((m + k) & 1U)
            result -= term;
        else
            result += term;
    }
    return result;
}

/* ---------- main function requested by the framework ---------- */
seq_t a(idx_t n)
{
    /* find the row index i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2 */
    idx_t lo = 0, hi = n * 2 + 2;          /* safe overestimate for hi */
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n) lo = mid;
        else        hi = mid - 1;
    }
    idx_t row = lo;
    __int128 pref = (__int128)row * (row + 1) / 2;
    idx_t col = n - (idx_t)pref;          /* column index */

    if (col > row) return (seq_t)0;       /* safety, though shouldn't happen */
    return compute_T(row, col);
}
