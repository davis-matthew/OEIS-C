/*
 * A273180  Numbers k such that ror(k) + rol(k) is a power of 2, where
 *          ror(k)=A038572(k) is k rotated one binary place to the right,
 *          rol(k)=A006257(k) is k rotated one binary place to the left.
 * https://oeis.org/A273180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 17*a(n-3) - 16*a(n-6) for n > 6 * with the initial values
 *            * a(1) = 1, a(2) = 2, a(3) = 6, * a(4) = 19, a(5) = 38, a(6) =
 *            102
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                /* size_t */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* behaviour for n < 1 is irrelevant */

    /* allocate an array of seq_t large enough for indices 0..n */
    size_t len = (size_t)n + 1;
    seq_t *seq = seq_alloc_new_array(len);
    if (seq == NULL)
        return (seq_t)0;           /* out of memory: return 0 (unlikely) */

    /* initialise all slots to zero */
    for (size_t i = 0; i < len; ++i)
        seq[i] = (seq_t)0;

    /* base cases 1..6 */
    if (n >= 1) seq[1] = (seq_t)1;
    if (n >= 2) seq[2] = (seq_t)2;
    if (n >= 3) seq[3] = (seq_t)6;
    if (n >= 4) seq[4] = (seq_t)19;
    if (n >= 5) seq[5] = (seq_t)38;
    if (n >= 6) seq[6] = (seq_t)102;

    /* recurrence for n > 6 */
    for (size_t i = 7; i <= (size_t)n; ++i)
    {
        seq[i] = seq[i-3] * (seq_t)17 - seq[i-6] * (seq_t)16;
    }

    seq_t result = seq[(size_t)n];
    seq_free_array(seq);             /* release unused memory */
    return result;
}
