/*
 * A242137  Numbers n such that there are no triangular numbers strictly
 *          between n^2 and n^2 + n.
 * https://oeis.org/A242137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer with no triangular number strictly between n²
 *            and n²+n. The routine tests integers sequentially and stores the
 *            results in a static dynamic array so that calls can arrive in
 *            any order
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root for 64‑bit unsigned integers */
static inline uint64_t _isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 4294967296ULL; /* 2^32 > sqrt(2^64‑1) */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) { lo = 1; continue; }
        if (mid <= x / mid)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi; /* floor sqrt */
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t next_m = 0; /* next integer to test */

    /* Expand storage lazily until we have at least n terms. */
    while (len < (size_t)n) {
        uint64_t m        = (uint64_t)next_m++;
        uint64_t t        = _isqrt_u64(8ULL * m * m + 1ULL);
        uint64_t k_floor  = (t - 1ULL) >> 1;
        uint64_t k_next   = k_floor + 1ULL;
        uint64_t T_next   = k_next * (k_next + 1ULL) / 2ULL;
        uint64_t upper    = m * m + m; /* m² + m */

        if (T_next >= upper) { /* m satisfies the condition */
            if (len == cap) {           /* grow capacity if needed */
                size_t newcap = cap ? (cap << 1) : 64;
                seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
                if (!tmp) {             /* allocation failed – abort */
                    exit(EXIT_FAILURE);
                }
                seq = tmp;
                cap = newcap;
            }
            seq[len++] = (seq_t)m;
        }
    }

    return seq[(size_t)n - 1];
}
