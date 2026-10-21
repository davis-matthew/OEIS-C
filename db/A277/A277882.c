/*
 * A277882  Dispersion of evil numbers, array transposed.
 * https://oeis.org/A277882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = T(i) + 2^(i-1)*(nonEvil(j)-1) where (i,j) are derived
 *            from the anti-diagonal enumeration of the transposed array of
 *            integers with odd (non-evil), odd-even (odd-evil) and even-even
 *            (even-evil) parity. For i≤3, T(i)=i*(i+1)/2; for i≥4,
 *            T(i)=3*2^(i-2)
 * Range:     u8 n <= 42, u16 n <= 150, u32 n <= 558, u64 n <= 2142,
 *            u128 n <= 8382, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_ROWS    200
#define MAX_NONEVIL 200

static seq_t rowStart[MAX_ROWS + 1];
static seq_t nonEvil[MAX_NONEVIL + 1];

static void init_arrays(void)
{
    /* first element of each row (triangular numbers up to i=3,
       then 3*2^(i-2) for i≥4) */
    rowStart[1] = (seq_t)1;
    rowStart[2] = (seq_t)3;
    rowStart[3] = (seq_t)6;
    rowStart[4] = (seq_t)12;
    for (idx_t r = 5; r <= MAX_ROWS; ++r)
        rowStart[r] = rowStart[r - 1] * (seq_t)2;

    /* compute the first MAX_NONEVIL non-evil (odd popcount) numbers */
    idx_t cnt = 0;
    for (uint64_t x = 1; cnt < MAX_NONEVIL; ++x) {
        if (__builtin_popcountll(x) & 1) {      /* odd number of 1-bits */
            ++cnt;
            nonEvil[cnt] = (seq_t)x;
        }
    }
}

static int initialized = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        init_arrays();
        initialized = 1;
    }

    /* find the diagonal d such that T(d)=d*(d+1)/2 ≥ n   */
    idx_t d = 1;
    idx_t tri = d * (d + 1) / 2;
    while (tri < n) {
        ++d;
        tri = d * (d + 1) / 2;
    }

    idx_t prev_tri = d * (d - 1) / 2;
    idx_t k     = n - prev_tri;      /* 1-based rank within the diagonal */
    idx_t i     = k;                 /* row index   (1 ≤ i ≤ d) */
    idx_t j     = d - k + 1;          /* column index(1 ≤ j ≤ d) */

    /* power of two: 2^(i-1) */
    seq_t pow2 = 1;
    for (idx_t p = 1; p < i; ++p)
        pow2 += pow2;                /* safely double for unsigned types */

    /* compute the term using the formula */
    seq_t term = rowStart[i] + ((nonEvil[j] - (seq_t)1) * pow2);
    return term;
}
