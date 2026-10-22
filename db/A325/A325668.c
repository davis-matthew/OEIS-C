/*
 * A325668  First term of n-th difference sequence of (floor(k*r)), r =
 *          sqrt(5), k >= 0.
 * https://oeis.org/A325668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = first term of the n‑th forward difference sequence of
 *            floor(k*sqrt(5)) for k=0,1,2,... Computed by building the
 *            sequence floor(k*sqrt(5)) and taking n successive finite
 *            differences
 * Range:     i8 n <= 9, i16 n <= 17, i32 n <= 35, i64 n <= 67, i128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root for 64‑bit unsigned arguments.
   Returns ⌊sqrt(x)⌋. */
static seq_t int_sqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 0xFFFFFFFFULL;   /* sqrt(UINT64_MAX) */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq = mid * mid;
        if (sq == x) return (seq_t)mid;
        if (sq < x)
            lo = mid + 1;
        else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return (seq_t)(lo - 1);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but harmless */

    size_t len = (size_t)n + 1;            /* number of elements needed */
    seq_t *v = (seq_t *)malloc(len * sizeof(seq_t));
    if (!v) abort();

    v[0] = 0;
    for (idx_t k = 1; k <= n; ++k) {
        uint64_t kk = (uint64_t)k;
        uint64_t val = 5 * kk * kk;       /* 5*k*k fits in 64‑bit for n≈10 000 */
        v[k] = int_sqrt_u64(val);         /* floor(sqrt(5*k*k)) */
    }

    /* n successive forward differences */
    size_t cur = len;
    for (size_t step = 0; step < (size_t)n; ++step) {
        for (size_t i = 0; i < cur - 1; ++i)
            v[i] = v[i + 1] - v[i];
        cur--;
    }

    seq_t result = v[0];
    free(v);
    return result;
}
