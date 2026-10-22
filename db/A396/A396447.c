/*
 * A396447  Square array A(n,k), n>=0, k>=0, read by antidiagonals downwards,
 *          where A(n,k) = [x^n] (F_k(x)/x)^(1/2) and F_k(x) is the k-th
 *          iteration of x*G4(x)^2 with G4(x) = 1 + x*G4(x)^4.
 * https://oeis.org/A396447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A396447: square array A(n,k) read anti‑diagonally.
 * 
 * Recurrence defining the square array:
 *   A(n,0) = (n==0 ? 1 : 0)
 *   A(n,k) = Sum_{j=0..n} ( 2*j+1 ) * binom(4*n-2*j+1 , n-j ) / (4*n-2*j+1) * A(j,k-1)   for k>0
 * 
 * The sequence a(n) is obtained by taking the anti‑diagonal reading of this array:
 *   Let d be the largest integer with d*(d+1)/2 <= n   (i.e. the diagonal number).
 *   offset = n - d*(d+1)/2
 *   a(n) = A( offset , d - offset )
 * 
 * The table needed to answer up to n=10000 only involves indices i+k <= 141, 
 * so a small triangular table is sufficient.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

#define MAX_D 200            /* sufficient for n <= 10000 (d <= 141) */

static seq_t A[MAX_D + 1][MAX_D + 1];  /* square array A(i,k) */
static idx_t computedD = 0;            /* largest diagonal already computed */
static int   initialized = 0;          /* flag for global initialization */

/* Integer square root: floor(sqrt(x)) for unsigned 64 bit x */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= (unsigned __int128)x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

/* Ensure that all A(i,k) with i+k <= d are computed */
static void init_table(void)
{
    if (initialized) return;
    /* Base column k=0 */
    for (idx_t i = 0; i <= MAX_D; ++i) {
        A[i][0] = (i == 0) ? (seq_t)1 : (seq_t)0;
    }
    initialized = 1;
}

/* Weight used in the recurrence */
static seq_t weight(idx_t i, idx_t j)
{
    idx_t a = 4*i - 2*j + 1;     /* 4*i - 2*j + 1 >= 1 */
    idx_t b = i - j;             /* 0 <= b <= i-j <= i */
    seq_t bin = binomial(a, b);
    seq_t num = (seq_t)(2*j + 1);
    seq_t den = (seq_t)a;
    return (bin * num) / den;
}

/* Build the triangular table up to diagonal d (if not already built) */
static void ensure_computed(idx_t d)
{
    init_table();
    if (d <= computedD) return;

    for (idx_t newd = computedD + 1; newd <= d; ++newd) {
        for (idx_t i = 0; i <= newd; ++i) {
            idx_t k = newd - i;          /* column index on this diagonal */
            if (k == 0) continue;        /* base column already set */
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j <= i; ++j) {
                seq_t w = weight(i, j);
                sum = sum + w * A[j][k - 1];
            }
            A[i][k] = sum;
        }
    }
    computedD = d;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute diagonal number d */
    idx_t tmp      = 8*n + (idx_t)1;
    idx_t sqrt_tmp = floor_sqrt_u64(tmp);
    idx_t d        = (sqrt_tmp - (idx_t)1) / (idx_t)2;

    /* Ensure the table contains all needed entries */
    ensure_computed(d);

    /* Off‑diagonal offset along this diagonal */
    idx_t sumprev = d * (d + (idx_t)1) / (idx_t)2;
    idx_t offset  = n - sumprev;      /* i == offset */
    idx_t i = offset;
    idx_t j = d - offset;             /* j == k column on this diagonal */

    return A[i][j];
}
