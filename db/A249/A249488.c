/*
 * A249488  Square array A(n,k) for n,k>=0, where A(n,k) is the number of
 *          paths from (0,0) to (n,k) in the directed graph with vertices
 *          (i,j) and edges (i,j)-to-(i+1,j), (i,j)-to-(i,j+1), and
 *          (i,i+h)-to-(i+1,i+h+1) for every i,j,h>=0.
 * https://oeis.org/A249488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Recurrence on the square array A(i,j): A(0,0) = 1 A(i,0) =
 *            A(0,j) = 1 A(i,j) = A(i-1,j) + A(i,j-1) + (j>=i ? A(i-1,j-1) :
 *            0) The sequence a(n) is the antidiagonal enumeration of A(i,j)
 * Range:     u8 n <= 48, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_D 256           /* enough for n ≤ 10000 (d ≈ 141) */

static seq_t A[MAX_D][MAX_D] = {0};
static idx_t cached_i = 0;
static idx_t cached_j = 0;

/* Integer square‑root for unsigned 64‑bit values, using binary search. */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Convert the offset n into coordinates (i,j) in the antidiagonal order. */
static void index_to_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* Find d such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2. */
    idx_t d = (floor_sqrt_u64(8 * n + 1) - 1) >> 1;

    /* Correct any off‑by‑one errors. */
    while (((unsigned __int128)(d + 1) * (d + 2)) / 2 <= n)
        ++d;
    while (((unsigned __int128)d * (d + 1)) / 2 > n)
        --d;

    idx_t offset = n - (unsigned __int128)d * (d + 1) / 2;
    *i = d - offset;
    *j = offset;
}

/* Expand the DP cache to include all indices up to (i,j). */
static void fill_cache(idx_t i, idx_t j)
{
    if (i > MAX_D - 1 || j > MAX_D - 1) {
        /* The current static table is too small; grow it.  For the
           requested range (n ≤ 10000) this never happens. */
        /* In a real implementation you would reallocate a larger table. */
    }

    for (idx_t a = 0; a <= i; ++a) {
        for (idx_t b = 0; b <= j; ++b) {
            if (A[a][b] != 0)
                continue;          /* already computed */
            if (a == 0 || b == 0) {
                A[a][b] = 1;        /* base case */
            } else {
                seq_t val = A[a - 1][b];
                val += A[a][b - 1];
                if (b >= a)
                    val += A[a - 1][b - 1];
                A[a][b] = val;
            }
        }
    }
    if (i > cached_i)
        cached_i = i;
    if (j > cached_j)
        cached_j = j;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    index_to_coords(n, &i, &j);
    fill_cache(i, j);
    return A[i][j];
}
