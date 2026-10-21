/*
 * A283433  Triangle read by rows: T(n,m) is the number of pattern classes in
 *          the (n,m)-rectangular grid with 4 colors and n>=m, two patterns
 *          are in the same class if one of them can be obtained by a
 *          reflection or 180-degree rotation of the other.
 * https://oeis.org/A283433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle defined by T(n,m) from
 *            OEIS A283433. T(n,m) is the number of 4‑color pattern classes in
 *            an n×m rectangle, up to reflections and a 180° rotation. The
 *            sequence is read row‑wise (triangular indexing)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow4.h"          /* seq_t pow4(idx_t exp); */

/* Integer square‑root for unsigned 64‑bit values (no floating point). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 32) - 1;   /* sqrt(2^64−1) < 2^32 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > x / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* Compute the triangle entry T(n,m).  n ≥ m assumed. */
static inline seq_t compute_T(idx_t n, idx_t m)
{
    idx_t prod = n * m;                 /* n*m  (fits in idx_t for typical n) */
    seq_t pow_mn = pow4(prod);          /* 4^(n*m) */

    bool n_even = (n & 1u) == 0;
    bool m_even = (m & 1u) == 0;

    if (n_even && m_even) {             /* both even */
        seq_t pow_half = pow4(prod / 2); /* 4^(mn/2) */
        return (pow_mn + pow_half * 3) / 4;
    }
    else if (n_even) {                  /* n even, m odd */
        seq_t pow_nm   = pow4((prod + n) / 2);   /* 4^((mn+n)/2) */
        seq_t pow_half = pow4(prod / 2);         /* prod is even */
        return (pow_mn + pow_nm + pow_half * 2) / 4;
    }
    else if (m_even) {                  /* n odd, m even */
        seq_t pow_mn_term = pow4((prod + m) / 2); /* 4^((mn+m)/2) */
        seq_t pow_half    = pow4(prod / 2);        /* prod is even */
        return (pow_mn + pow_mn_term + pow_half * 2) / 4;
    }
    else {                              /* both odd */
        seq_t pow_nm = pow4((prod + n) / 2);     /* 4^((mn+n)/2) */
        seq_t pow_mn_term = pow4((prod + m) / 2);/* 4^((mn+m)/2) */
        seq_t pow_one = pow4((prod + 1) / 2);     /* 4^((mn+1)/2) */
        return (pow_mn + pow_nm + pow_mn_term + pow_one) / 4;
    }
}

/* Main sequence accessor.  idx is the global index in the triangular
   reading (a(0) = T(0,0), a(1) = T(1,0), a(2) = T(1,1), etc.). */
seq_t a(idx_t idx)
{
    /* Find row r with r*(r+1)/2 <= idx < (r+1)*(r+2)/2. */
    idx_t tri  = 8 * idx + 1;          /* 8*idx+1 fits in idx_t for typical limits */
    idx_t root = isqrt_u64(tri);
    idx_t r    = (root - 1) / 2;        /* row index, 0‑based */

    /* Column index m (0 <= m <= r). */
    idx_t prev = r * (r + 1) / 2;       /* number of entries before row r */
    idx_t m    = idx - prev;

    return compute_T(r, m);
}
