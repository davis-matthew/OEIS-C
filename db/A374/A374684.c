/*
 * A374684  Sum of leaders of strictly increasing runs in the n-th composition
 *          in standard order.
 * https://oeis.org/A374684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of leaders of strictly increasing runs in the n‑th
 *            composition in standard order (lexicographically with larger
 *            parts first, and compositions ordered by increasing sum)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Find the sum S of the n‑th composition.
       S is the smallest integer such that 2^S - 1 >= n. */
    idx_t pow2 = 1;        /* 2^0 */
    idx_t s = 0;           /* current exponent */
    while (pow2 <= n) {
        pow2 <<= 1;
        ++s;
    }

    /* offset within compositions of sum s (0‑based). */
    idx_t offset = n - (pow2 >> 1);  /* 2^(s-1) = pow2/2 */

    /* Decode offset into the parts of the composition.
       The binary representation of offset (length s-1) gives the
       separator positions: 1 means cut after that element. */
    seq_t parts[65];        /* maximum possible parts (s <= 64 for idx_t) */
    idx_t count = 0;
    seq_t len = 1;
    for (idx_t i = 0; i < s - 1; ++i) {
        idx_t bit = (offset >> (s - 2 - i)) & 1;
        if (bit) {
            parts[count++] = len;
            len = 1;
        } else {
            ++len;
        }
    }
    parts[count++] = len;   /* last part */

    /* Compute sum of leaders of strictly increasing runs.
       A new run starts when the current part is not strictly greater
       than the previous part (i.e., <=). */
    seq_t sum = 0;
    seq_t prev = 0;          /* will be overwritten on first iteration */
    for (idx_t i = 0; i < count; ++i) {
        seq_t val = parts[i];
        if (i == 0 || val <= prev) {
            sum += val;
        }
        prev = val;
    }
    return sum;
}
