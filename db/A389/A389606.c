/*
 * A389606  Triangle T(n,k) read by rows where T(n,k) = (n+1)*(1+(-1)^x)/2 -
 *          k*(-1)^x where x = (n-1)/2 - |k-(n+1)/2|.
 * https://oeis.org/A389606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read row-wise from the triangle T(n,k) defined by T(n,k)
 *            = (n+1)*(1+(-1)^x)/2 - k*(-1)^x, where x = (n-1)/2 -
 *            |k-(n+1)/2|. The sequence starts at a(1)=1
 * Range:     u8 n <= 8960, u16 n <= 8960, u32 n <= 8960, u64 n <= 8960,
 *            u128 n <= 8960, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, so n>=1. */

    /* Find row number r such that r(r-1)/2 < n <= r(r+1)/2. */
    uint64_t j = (uint64_t)n;        /* current index, 1‑based. */
    uint64_t r = (uint64_t)((sqrt(8.0 * (double)j + 1.0) - 1.0) / 2.0);

    /* Correct possible off‑by‑one due to rounding. */
    while ((r * (r + 1)) / 2 < j) ++r;
    while (r > 0 && ((r - 1) * r) / 2 >= j) --r;

    uint64_t prev = (r * (r - 1)) / 2;   /* elements before row r. */
    uint64_t k = j - prev;              /* position in the row, 1‑based. */

    /* Compute the exponent x. */
    int64_t x;
    if (r & 1ULL) {                     /* r is odd */
        uint64_t m = r / 2ULL;                  /* m = (r-1)/2            */
        uint64_t center = m + 1ULL;              /* center = (r+1)/2       */
        uint64_t diff = (k > center) ? (k - center) : (center - k);
        x = (int64_t)m - (int64_t)diff;
    } else {                             /* r is even */
        uint64_t m = r / 2ULL;                  /* r = 2m                 */
        int64_t two_k = (int64_t)k << 1;        /* 2*k                   */
        int64_t b = two_k - (int64_t)(2ULL * m + 1ULL);
        if (b < 0) b = -b;                     /* abs difference        */
        int64_t a_val = (int64_t)(2ULL * m - 1ULL);  /* 2m-1                 */
        x = (a_val - b) >> 1;                  /* (a-b)/2               */
    }

    int64_t s = (x & 1LL) ? -1LL : 1LL;        /* (-1)^x                */

    /* Compute T(r,k) in signed arithmetic. */
    int64_t term = ((int64_t)r + 1) * (1 + s) / 2 - (int64_t)k * s;

    return (seq_t)term;
}
