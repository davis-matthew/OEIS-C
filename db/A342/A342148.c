/*
 * A342148  Infinite square matrix A(m,n) = F(m) mod n, m,n >= 1, where F =
 *          Fibonacci = A000045, read by falling antidiagonals.
 * https://oeis.org/A342148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n-th element, read by falling antidiagonals,
 * of the infinite matrix A(m,n) = F(m) mod n  (F = Fibonacci).
 *
 * Implementation notes:
 *   - The diagonal index k is the smallest integer such that k(k+1)/2 ≥ n.
 *   - On this diagonal the i‑th term (i=1 … k) corresponds to
 *     (m = i,  j = k-i+1)  and its value is
 *         0                       if j == 1 or F(m) == j,
 *         F(m)                    if F(m) < j,
 *         F(m) mod j              otherwise.
 *   - We use the library routine fib(idx_t) to obtain the exact
 *     Fibonacci number F(m).  The function is provided by the
 *     automatic OEIS utilities.
 *   - All arithmetic on indices is performed on idx_t (uint64_t),
 *     and Fibonacci values are in seq_t (an unsigned integer type
 *     suitable for the build, possibly big‑int).
 *   - A binary search is used to find k; it requires only 128‑bit
 *     arithmetic for safety, which is available in C++ and in C11
 *     via `unsigned __int128`.
 */

#include <stdint.h>
#include "../utils/fib.h"

static idx_t diagonal_index(idx_t n)
{
    /* Find smallest k such that k*(k+1)/2 >= n.
       Binary search on the 64‑bit range. */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find the diagonal (k) and position (t) on that diagonal. */
    idx_t k = diagonal_index(n);
    unsigned __int128 before = (unsigned __int128)(k - 1) * k / 2;
    idx_t t = n - (idx_t)before;           /* 1‑based position in diagonal */
    idx_t m = t;                           /* row index */
    idx_t j = k - t + 1;                   /* column index */

    seq_t F = fib(m);                      /* Fibonacci number F(m) */

    if (j == 1)                /* column 1 gives 0 */
        return 0;
    if (F == (seq_t)j)         /* F(m) equals column index -> 0 */
        return 0;
    if (F < (seq_t)j)          /* column larger than Fibonacci -> unchanged */
        return F;
    /* normal modulus */
    return F % (seq_t)j;
}
