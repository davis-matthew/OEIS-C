/*
 * A368434  Irregular triangular array T, read by rows: T(n,k) = number of
 *          sums |x-y|+|y-z| = k, where x,y,z are in {1,...,n}.
 * https://oeis.org/A368434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r = ceil(sqrt(n)) and k = n - (r-1)^2 - 1
 *            (0‑based index into row r). For row r, T(r,k) = Σ_{d1+d2=k}
 *            S(r,d1,d2), where S(r,d1,d2) = Σ_{y=1}^r f(y,d1)·f(y,d2) and
 *            f(y,d) = |{x∈[1,r] : |x-y|=d}|. The implementation uses a
 *            closed‑form expression for S(r,d1,d2)
 * Range:     u8 n <= 204, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sum_f.h"

/* ---- integer square root (floor) ----------------------------------- */
static inline idx_t floor_sqrt(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = 0xFFFFFFFFULL;          /* > sqrt(max_uint64) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1; /* upper mid */
        if (mid > x / mid) hi = mid - 1;
        else               lo = mid;
    }
    return lo;
}

static inline idx_t ceil_sqrt(idx_t x)
{
    if (x == 0) return 0;
    idx_t f = floor_sqrt(x);
    if (f * f == x) return f;
    return f + 1;
}

/* ---- S(r,d1,d2) ----------------------------------------------------- */
static inline seq_t compute_S(idx_t n, idx_t d1, idx_t d2)
{
    if (d1 > n - 1 || d2 > n - 1) return 0;
    if (d1 == 0 && d2 == 0) return (seq_t)n;
    if (d1 == 0) return sum_f(n, d2);
    if (d2 == 0) return sum_f(n, d1);

    /* both d1,d2 > 0 */
    idx_t maxd = (d1 > d2) ? d1 : d2;
    idx_t term1 = n - maxd;                 /* always ≥ 1 */

    idx_t temp = 0;
    if (n > d1 + d2) temp = n - d1 - d2;    /* otherwise zero */

    return (seq_t)2 * (seq_t)term1 + (seq_t)2 * (seq_t)temp;
}

/* ---- T(r,k) -------------------------------------------------------- */
static inline seq_t T_row_k(idx_t n, idx_t k)
{
    seq_t sum = 0;
    for (idx_t d = 0; d <= k; ++d) {
        idx_t d1 = d;
        idx_t d2 = k - d;
        sum += compute_S(n, d1, d2);
    }
    return sum;
}

/* ---- public sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    /* n is 1‑based index in the flattened sequence */
    idx_t r   = ceil_sqrt(n);
    idx_t prev= (r - 1) * (r - 1);          /* elements before row r */
    idx_t k   = n - prev - 1;               /* 0‑based index within row */

    return T_row_k(r, k);
}
