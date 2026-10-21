/*
 * A259575  Reciprocity array of 1; rectangular, read by antidiagonals.
 * https://oeis.org/A259575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(m, n0) where (m, n0) is the pair on the anti‑diagonal
 *            that contains the nth term (1‑based) when the array T(m, n) is
 *            read by anti‑diagonals starting from (1,1). T(m, n) =
 *            Σ_{k=0}^{m-1} ⌊(n*k+1)/m⌋
 * Range:     u8 n <= 1100, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the anti‑diagonal index s (m + n0) such that
   s*(s-1)/2 >= n.  (s >= 2) */
static idx_t find_s(idx_t n)
{
    idx_t low = 2;
    idx_t high = 2;
    /* Grow high until its triangular number reaches or exceeds n */
    for (;;) {
        unsigned __int128 val = (__int128)high * (high - 1) / 2;
        if (val >= n) break;
        if (high >= (UINT64_MAX >> 1)) { /* avoid shift overflow */
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }
    /* Binary search for the smallest s with s*(s-1)/2 >= n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((__int128)mid * (mid - 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1 – return 0 gracefully */

    /* Step 1: locate the anti‑diagonal s with m + n0 = s. */
    idx_t s = find_s(n);

    /* Step 2: compute offset inside that diagonal (0‑based). */
    unsigned __int128 prev = (__int128)(s - 1) * (s - 2) / 2;  /* C(s-1) */
    idx_t offset = (idx_t)(n - prev - 1);

    /* Coordinates on the diagonal: (m, n0) = (offset+1, s-offset-1). */
    idx_t m   = offset + 1;
    idx_t n0  = s - m;

    /* Step 3: evaluate T(m, n0). */
    seq_t sum = 0;
    seq_t m_u  = (seq_t)m;
    seq_t n0_u = (seq_t)n0;

    for (idx_t k = 0; k < m; ++k) {
        seq_t term = (n0_u * (seq_t)k + 1) / m_u;
        sum += term;
    }

    return sum;
}
