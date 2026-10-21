/*
 * A286235  Triangular table T(n,k) = P(phi(k), floor(n/k)), where P is
 *          sequence A000027 used as a pairing function N x N -> N, and phi is
 *          Euler totient function, A000010. Table is read by rows as T(1,1),
 *          T(2,1), T(2,2), etc.
 * https://oeis.org/A286235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ceil((sqrt(1+8n)-1)/2), k = n -
 *            r(r-1)/2, and T(r,k) = (1/2)*(2 + ((phi(k)+floor(r/k))^2) -
 *            phi(k) - 3*floor(r/k))
 * Range:     u8 n <= 276, u16 n <= 9652, u32 n <= 9652, u64 n <= 9652,
 *            u128 n <= 9652, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"

/* Compute the row index r for a given overall index n. */
static idx_t compute_row(idx_t n)
{
    idx_t low = 0, high = n;          /* r cannot exceed n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return high;                       /* minimal r with r(r+1)/2 >= n */
}

seq_t a(idx_t n)
{
    /* Find the row r and column k (within that row). */
    idx_t r = compute_row(n);
    unsigned __int128 prev_prod = (unsigned __int128)(r - 1) * r;
    idx_t prev = (idx_t)(prev_prod / 2);        /* total elements before row r */
    idx_t k = n - prev;                        /* column index (1‑based) */

    /* Compute φ(k) and floor(r/k). */
    seq_t phi_k = phi(k);
    seq_t y = (seq_t)(r / k);                  /* floor(r/k) fits in idx_t */

    /* Apply the given pairing formula. */
    seq_t sum = phi_k + y;
    seq_t val = ((sum * sum) - phi_k - 3 * y + 2) / 2;

    return val;
}
