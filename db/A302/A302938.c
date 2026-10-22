/*
 * A302938  Lexicographically first sequence of distinct terms such that the
 *          sum of any two terms is not a term of the sequence, and the sum of
 *          any two digits is not a digit of the sequence.
 * https://oeis.org/A302938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1, a(2)=2; for n≥3, let k=n-2. Then a(n) is obtained by
 *            writing k+1 in binary, dropping the leading 1 bit, and replacing
 *            0→4, 1→7. (This is equivalent to A284971(k).)
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 1024, u64 n <= 9875,
 *            u128 n <= 9875, big any n
 * Helpers:   map_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/map_index.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;
    /* n ≥ 3 */
    idx_t k = n - (idx_t)2;   /* k ≥ 1 */
    return map_index(k);
}
