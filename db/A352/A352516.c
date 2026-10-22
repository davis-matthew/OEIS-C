/*
 * A352516  Number of excedances (parts above the diagonal) of the n-th
 *          composition in standard order.
 * https://oeis.org/A352516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ i | part_i > i }| where (part_1,…,part_k) is the nth
 *            composition in standard order (graded reverse‑lexicographic)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation follows the definition:
   - take the set of positions of 1‑bits in the reversed binary
     expansion of n (positions counted from 1),
   - prepend 0,
   - take successive differences,
   - reverse the sequence of differences to obtain the composition. */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>
#include <stdint.h>   /* may be useful for idx_t definition */

seq_t a(idx_t n)
{
    if (n == 0U)                /* empty composition – no excedances */
        return 0U;

    /* The binary representation of n has at most 64 bits, so
       at most 64 parts.  Use a fixed array. */
    seq_t parts[65];
    size_t m = 0;               /* number of parts */
    idx_t prev = 0;             /* previous 1‑bit position (1‑based) */
    idx_t bitpos = 0;           /* current bit index (0‑based) */
    idx_t temp = n;

    /* Find positions of 1‑bits, compute successive differences. */
    while (temp != 0) {
        if (temp & 1U) {        /* bit is set */
            idx_t pos = bitpos + 1;     /* 1‑based position */
            parts[m++] = (seq_t)(pos - prev);
            prev = pos;
        }
        temp >>= 1;
        ++bitpos;
    }

    /* Count excedances: reverse order of parts, compare with index. */
    seq_t count = 0;
    for (size_t i = 0; i < m; ++i) {
        seq_t part = parts[m - 1 - i];
        if (part > (seq_t)(i + 1))
            ++count;
    }

    return count;
}
