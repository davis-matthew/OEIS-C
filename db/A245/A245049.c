/*
 * A245049  Number A(n,k) of hybrid k-ary trees with n internal nodes; square
 *          array A(n,k), n>=0, k>=1, read by antidiagonals.
 * https://oeis.org/A245049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,k) = 1/((k-1)*n+1) * Σ_{i=0..n} C((k-1)*n+i,i) *
 *            C((k-1)*n+i+1,n-i) a(0) = A(0,1), a(1) = A(0,2), a(2) = A(1,1),
 *            … (anti‑diagonal read)
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 99, u64 n <= 244,
 *            u128 n <= 618, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include <stdint.h>

#if defined(__cplusplus)
#include <type_traits>
static seq_t compute_A(idx_t n, idx_t k)
{
    idx_t denom_idx = (k - 1) * n + 1;
    seq_t denom = (seq_t)denom_idx;

    if constexpr (std::is_integral<seq_t>::value) {
        /* 64‑bit or smaller: use 128‑bit arithmetic for exactness          */
        using big = unsigned __int128;
        big sum = 0;
        for (idx_t i = 0; i <= n; ++i) {
            seq_t t1 = binom((k - 1) * n + i, i);
            seq_t t2 = binom((k - 1) * n + i + 1, n - i);
            sum += (big)t1 * (big)t2;
        }
        return (seq_t)(sum / denom);
    } else {
        /* arbitrary‑precision class: built‑in multiplication is safe      */
        seq_t sum = 0;
        for (idx_t i = 0; i <= n; ++i) {
            seq_t t1 = binom((k - 1) * n + i, i);
            seq_t t2 = binom((k - 1) * n + i + 1, n - i);
            sum += t1 * t2;
        }
        return sum / denom;
    }
}
#else
/* C implementation – seq_t is an unsigned 8/16/32/64‑bit type            */
static seq_t compute_A(idx_t n, idx_t k)
{
    idx_t denom_idx = (k - 1) * n + 1;
    seq_t denom = (seq_t)denom_idx;

    unsigned __int128 sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t t1 = binom((k - 1) * n + i, i);
        seq_t t2 = binom((k - 1) * n + i + 1, n - i);
        sum += (unsigned __int128)t1 * (unsigned __int128)t2;
    }
    return (seq_t)(sum / denom);
}
#endif

/* Map the linear index n to coordinates (internal_nodes, k) in the square array.
   The array is read anti‑diagonally:  A(0,1), A(0,2), A(1,1), A(0,3), A(1,2), A(2,1), …   */
seq_t a(idx_t N)
{
    idx_t d = 0;                      /* will be the diagonal number minus one */
    while ((d + 1) * (d + 2) / 2 <= N)  /* find the largest d with cumulative ≤ N */
        ++d;

    idx_t prev = d * (d + 1) / 2;   /* cumulative count up to previous diagonal */
    idx_t offset = N - prev;        /* offset inside the current diagonal   */

    idx_t internal = offset;           /* n                */
    idx_t k = d - offset + 1;          /* k = d - offset   */

    return compute_A(internal, k);
}
