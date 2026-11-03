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
 * Range:     u8 n <= 8, u16 n <= 48, u32 n <= 1024, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 66 terms through n = 9875 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t map_index(idx_t k)
{
    /* k ≥ 1 */
    idx_t valbits = k + 1;            /* the binary value to encode */
    /* find position of most significant bit */
    int msb = 0;
    idx_t tmp = valbits;
    while (tmp > 1) {
        tmp >>= 1;
        ++msb;
    }
    /* msb is the index of the highest set bit (0‑based) */
    seq_t res = (seq_t)0;
    for (int b = msb - 1; b >= 0; --b) {
        res = res * (seq_t)10;
        res += (valbits & ((idx_t)1 << b)) ? (seq_t)7 : (seq_t)4;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;
    /* n ≥ 3 */
    idx_t k = n - (idx_t)2;   /* k ≥ 1 */
    return map_index(k);
}
