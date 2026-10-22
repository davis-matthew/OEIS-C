/*
 * A341216  Triangle read by columns T(n,k) k > n >= 1: Last survivor
 *          positions in a modified Josephus problem for n numbers, where
 *          after each deletion the counting starts over at the lowest
 *          existing number n, rather than continuing from the current
 *          position.
 * https://oeis.org/A341216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read column‑wise from the triangle T(n,k) defined by
 *            T(1,k) = 1 (k > 1) T(n,k) = T(n-1,k) if k mod n > T(n-1,k) or k
 *            mod n == 0 T(n,k) = T(n-1,k) + 1 otherwise
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is obtained by listing T(1,2), T(1,3), T(2,3), T(1,4), T(2,4),
   T(3,4), … */

#include <math.h>  /* sqrt */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* not defined, return 0 */

    /* Find the column k such that k*(k-1)/2 >= n.
       We first approximate k using the quadratic formula and then correct. */
    double d = sqrt((double)1 + 8.0 * (double)n);
    idx_t k = (idx_t)((1.0 + d) / 2.0);          /* floor of the solution */
    if (((unsigned __int128)k * (k - 1)) / 2 < n)
        ++k;

    /* Compute r: position within column k (1-based). */
    idx_t prev = (k >= 2) ? ( (unsigned __int128)(k - 1) * (k - 2) / 2 ) : 0ULL;
    idx_t r = n - prev;   /* 1 <= r <= k-1 */

    /* Evaluate T(r, k) by iterating the recurrence. */
    seq_t val = 1;  /* T(1, k) */
    for (idx_t i = 2; i <= r; ++i) {
        idx_t mod = k % i;
        if (mod == 0 || mod > (idx_t)val) {
            /* val unchanged */
        } else {
            val += (seq_t)1;
        }
    }
    return val;
}
