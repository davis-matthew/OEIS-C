/*
 * A319278  Square array sigma_k(n) read down antidiagonals: sum of the k-th
 *          powers of the divisors of n.
 * https://oeis.org/A319278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = σ_{k(m)}(m) where the pair (m,k) is obtained by reading
 *            the square array σ_k(n) = ∑_{d|n} d^k down its antidiagonals.
 *            I.e. a(n) is the n‑th element when the rows of the array are
 *            traversed by antidiagonals (m+k constant) from top to bottom
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

/* Compute a single term of A319278.  Uses a binary search to
   recover the (m,k) pair from the linear index n, then calls
   the supplied utility sigma_k(m,k). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset is 1, unused for 0 */

    /* Find the smallest s ≥ 2 such that (s-1)*s/2 ≥ n.
       s is the sum m+k for the antidiagonal that contains position n. */
    idx_t low = 2;
    idx_t high = n + 1;                         /* upper bound */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 cur = (unsigned __int128)(mid - 1) * mid / 2;
        if (cur >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    /* Number of elements preceding this antidiagonal. */
    unsigned __int128 prev128 = (unsigned __int128)(s - 2) * (s - 1) / 2;
    idx_t prev = (idx_t)prev128;

    /* Position (1‑based) within the current antidiagonal. */
    idx_t pos = n - prev;

    /* Recover m and k.  m = pos, k = s - pos. */
    idx_t m = pos;
    idx_t k = s - pos;                          /* k ≥ 1 */

    /* k may exceed 255 for very large n; the sequence test values
       are only up to a few thousand, so casting to uint8_t is safe. */
    return sigma_k(m, (uint8_t)k);
}
