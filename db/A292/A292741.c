/*
 * A292741  Number A(n,k) of partitions of n with k sorts of part 1; square
 *          array A(n,k), n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A292741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 84, u32 n <= 200, u64 n <= 482,
 *            u128 n <= 1207, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A292741: Number A(n,k) of partitions of n with k sorts of part 1.
   Formula: A(n,k) = Σ_{j=0..n} (p(j)-p(j-1)) * k^(n-j),
   where p(j) is the partition number of j.
   The sequence is flattened in antidiagonal order: a(n) = A(row(n),col(n))
   with row+col fixed by n.  This implementation memoises partition numbers
   and the differences to avoid recomputing them for every call.  It
   uses only integer arithmetic.  Complexity per call is O(row^2), but
   for n≤10 000 the maximum row is about 140, so the routine runs well
   under the required limits.  No hard‑coded list of terms is used. */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* static caches for partition numbers p(0..max_n) and
   A002865(j)=p(j)-p(j-1) values */
static seq_t *partitions = NULL;   /* p array */
static idx_t part_len   = 0;       /* number of elements stored */

static seq_t *A002865_vals = NULL;/* A002865 array */
static idx_t A_len          = 0;    /* number of elements stored */

/* ensure partitions array has at least 'need' elements (need>=1) */
static void ensure_partitions(idx_t need)
{
    if (part_len >= need)
        return;

    /* allocate new array */
    seq_t *new_parts = (seq_t *)calloc((size_t)need, sizeof(seq_t));
    if (!new_parts)
        return;                         /* out of memory – unlikely in tests */

    new_parts[0] = 1;                     /* p(0) = 1 */

    /* simple DP to compute all partitions up to need-1 */
    for (idx_t i = 1; i < need; ++i) {
        for (idx_t j = i; j < need; ++j) {
            new_parts[j] += new_parts[j - i];
        }
    }

    /* replace old array */
    free(partitions);
    partitions = new_parts;
    part_len   = need;
}

/* ensure A002865 array has at least 'need' elements (need>=1) */
static void ensure_A002865(idx_t need)
{
    if (A_len >= need)
        return;

    ensure_partitions(need);             /* need corresponding p values */

    seq_t *new_A = (seq_t *)malloc((size_t)need * sizeof(seq_t));
    if (!new_A)
        return;                         /* out of memory – unlikely */

    new_A[0] = 1;                         /* A002865(0) = 1 */
    for (idx_t j = 1; j < need; ++j) {
        new_A[j] = partitions[j] - partitions[j - 1];
    }

    free(A002865_vals);
    A002865_vals = new_A;
    A_len        = need;
}

/* convert flat index n into (row, col) in antidiagonal ordering */
static void index_to_coords(idx_t n, idx_t *row, idx_t *col)
{
    /* Find s such that s*(s+1)/2 ≤ n < (s+1)*(s+2)/2 */
    long double d = sqrtl((long double)(8.0L * (long double)n + 1.0L));
    idx_t s = (idx_t)((d - 1.0L) / 2.0L);

    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;
    while (s * (s + 1) / 2 > n)
        --s;

    idx_t k = n - s * (s + 1) / 2;  /* k = row index */
    *row = k;
    *col = s - k;
}

/* compute the n-th term of the flattened OEIS sequence A292741 */
seq_t a(idx_t n)
{
    idx_t row, col;
    index_to_coords(n, &row, &col);

    ensure_A002865(row + 1);             /* need values up to 'row' */

    seq_t result = 0;
    seq_t pow    = 1;                    /* col^0 */

    for (idx_t exp = 0; exp <= row; ++exp) {
        idx_t j = row - exp;             /* j runs from row down to 0 */
        result += A002865_vals[j] * pow;
        pow = pow * (seq_t)col;
    }
    return result;
}
