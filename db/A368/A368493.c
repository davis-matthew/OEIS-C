/*
 * A368493  T(n,m) is the number of m-dimensional isotropic subspaces of a
 *          2n-dimensional symplectic space over Z/2, n >= 0 and 0 <= m <= n.
 * https://oeis.org/A368493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,m) = ∏_{i=n‑m+1..n} (2^(2i)-1) / ∏_{i=1..m} (2^i-1) The
 *            sequence a(k) is the anti‑diagonal enumeration of T(n,m)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 30, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Triangular number t(n) = n*(n+1)/2, computed using 128‑bit intermediate. */
static inline uint64_t triangular(uint64_t n) {
    return (uint64_t)(((unsigned __int128)n * (n + 1)) >> 1);
}

/* Convert index k to (n,m) where a(k) = T(n,m) and m = k - n*(n+1)/2. */
static inline void index_to_nm(uint64_t k, uint64_t *n_out, uint64_t *m_out) {
    uint64_t low = 0, high = 1;
    /* Find an upper bound for n: keep shifting until triangular(high) > k. */
    while (triangular(high) <= k) {
        low = high;
        high <<= 1;
        if (high == 0) break;   /* avoid infinite shift at max value */
    }
    /* Binary search for the largest n with triangular(n) <= k. */
    while (low < high) {
        uint64_t mid = low + ((high - low + 1) >> 1);
        if (triangular(mid) <= k)
            low = mid;
        else
            high = mid - 1;
    }
    *n_out = low;
    *m_out = k - triangular(low);
}

/* Main sequence function */
seq_t a(idx_t idx) {
    uint64_t k = (uint64_t)idx;
    uint64_t n, m;
    index_to_nm(k, &n, &m);

    seq_t result = 1;
    /* Compute T(n,m) multiplicatively: result = Π num / den for i = 0 .. m-1. */
    for (uint64_t i = 0; i < m; ++i) {
        seq_t numerator   = pow2(2 * (n - i)) - 1;
        seq_t denominator = pow2(i + 1)      - 1;
        result *= numerator;
        result /= denominator;   /* division is exact after the multiplication */
    }
    return result;
}
