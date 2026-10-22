/*
 * A392782  Complement of A000792.
 * https://oeis.org/A392782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392782: Complement of A000792 (numbers not of the form 3^k,
 * 2·3^k, or 4·3^k).  a(1)=5, a(2)=7, a(3)=8, a(4)=10, a(5)=11, …  */

/* Return the n‑th positive integer that is not of the form 3^k,
 * 2·3^k, or 4·3^k.  The index n starts at 1. */
#include <stdbool.h>

static inline bool is_excluded(idx_t x)
{
    idx_t e = 0;
    /* strip factors of 2 */
    while ((x & 1u) == 0u) {
        x >>= 1u;
        ++e;
    }
    if (e > 2)             /* 8·3^k and higher powers of 2 are never excluded */
        return false;

    /* now x is odd; check if it is a pure power of 3 */
    if (x == 1u)           /* corresponds to 3^0, i.e., 1, 2, or 4 */
        return true;
    while (x % 3u == 0u)
        x /= 3u;
    return x == 1u;
}

seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many allowed numbers found */
    idx_t candidate = 1;      /* current integer to test */

    while (count < n) {
        if (!is_excluded(candidate))
            ++count;
        ++candidate;
    }
    /* candidate was incremented once after reaching count==n */
    return (seq_t)(candidate - 1);
}
