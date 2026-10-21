/*
 * A253669  Square array read by ascending antidiagonals, T(n, k) =
 *          k!*[x^k](log(x+1)*sum(j=0..n, C(2*n,j)*x^j)), n>=0, k>=0.
 * https://oeis.org/A253669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(n',k') read by ascending antidiagonals, where T(n,k) =
 *            k! * [x^k]( log(1+x) * sum_{j=0..n} C(2n,j) x^j ). The mapping
 *            from index n to (n',k') is: let D = floor((sqrt(8n+1)-1)/2); let
 *            pos = n - D*(D+1)/2; // 0 <= pos <= D then n' = D - pos, k' =
 *            pos
 * Range:     i8 n <= 26, i16 n <= 43, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root for unsigned 64-bit values. */
static unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long r = 0;
    unsigned long long bit = 1ULL << 62;   /* highest power of four <= 2^64 */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Find diagonal D and position pos */
    unsigned long long tmp = 8ULL * n + 1ULL;
    unsigned long long s = isqrt_u64(tmp);
    idx_t D = (idx_t)((s - 1ULL) / 2ULL);

    idx_t tot_before = D * (D + 1) / 2;
    idx_t pos = n - tot_before;     /* 0 <= pos <= D */

    idx_t k = pos;                  /* column index */
    idx_t row = D - pos;            /* row index n' */

    if (k == 0)                     /* T(*,0) = 0 */
        return (seq_t)0;

    /* factorial of k */
    seq_t fact = 1;
    for (idx_t i = 1; i <= k; ++i)
        fact = fact * (seq_t)i;

    /* Compute binomial coefficients C(2*row, j) for j = 0..row */
    seq_t *c = (seq_t*)malloc((row + 1) * sizeof(seq_t));
    c[0] = 1;
    for (idx_t j = 1; j <= row; ++j) {
        seq_t num = (seq_t)(2 * row - (j - 1));
        c[j] = c[j - 1] * num / (seq_t)j;
    }

    /* Sum over m = 1..k of (-1)^(m+1) * C(row, k-m) * k! / m */
    seq_t sum = 0;
    for (idx_t m = 1; m <= k; ++m) {
        idx_t j = k - m;                 /* coefficient index */
        if (j > row) continue;          /* coefficient is 0 for j > row */
        seq_t cval = c[j];
        seq_t term = cval * fact / (seq_t)m;
        if ((m & 1) == 0)              /* even m => sign - */
            term = -term;
        sum += term;
    }

    free(c);
    return sum;
}
