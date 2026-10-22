/*
 * A352362  Array read by ascending antidiagonals. T(n, k) = L(k, n) where L
 *          are the Lucas polynomials.
 * https://oeis.org/A352362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = T(i, j) where i = S - (n - S(S+1)/2), j = n - S(S+1)/2, S
 *            = floor((√(8n+1)-1)/2), T(n,0)=2, T(n,1)=n and for k≥2 T(n,k) =
 *            n·T(n,k-1) + T(n,k-2). The array of Lucas polynomials is read by
 *            ascending antidiagonals with decreasing i on each diagonal
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   lucas_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/lucas_value.h"

/* Find the diagonal S such that S(S+1)/2 ≤ n < (S+1)(S+2)/2. */
static inline idx_t find_diag(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Expand high until too large */
    while ((__uint128_t)high * (high + 1) / 2 <= n)
        high <<= 1;
    /* Binary search for maximal S with S(S+1)/2 ≤ n */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((__uint128_t)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find diagonal S and offset within it. */
    idx_t S = find_diag(n);
    __uint128_t t = (__uint128_t)S * (S + 1) / 2;
    idx_t total_before = (idx_t)t;
    idx_t p = n - total_before;   /* position on diagonal, 0 … S */
    idx_t i = S - p;              /* row index (decreasing on the diagonal) */
    idx_t j = p;                  /* column index */

    /* Compute T(i,j). i fits in idx_t but is cast to seq_t for multiplication. */
    return lucas_value((seq_t)i, j);
}
