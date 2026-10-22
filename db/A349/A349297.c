/*
 * A349297  Triangle T(n,k) = 1 if both n and k are even or if n and k are
 *          divisible by 3.
 * https://oeis.org/A349297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where r and k are obtained from n by *
 *            interpreting n as a flattened triangular array: 1, (1,2), *
 *            (1,2,3),
 * Range:     u8 n <= 9708, u16 n <= 9708, u32 n <= 9708, u64 n <= 9708,
 *            u128 n <= 9708, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first entry.  n = 0 is not used
     * but we return 0 to be safe. */
    if (n == 0) return (seq_t)0;

    /* Find the row r such that 1 + 2 + … + r >= n. */
    idx_t r = 0;
    idx_t cum = 0;            /* cumulative entries up to current r */
    while (cum < n) {
        r++;
        cum += r;
    }

    /* Column k (1‑based) within this row. */
    idx_t k = n - (cum - r);

    /* Evaluate triangle entry T(r,k). */
    if ((r & 1ULL) == 0 && (k & 1ULL) == 0)   /* both even */
        return (seq_t)1;
    if ((r % 3ULL) == 0 && (k % 3ULL) == 0)   /* both divisible by 3 */
        return (seq_t)1;
    return (seq_t)0;
}
