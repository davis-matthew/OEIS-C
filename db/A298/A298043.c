/*
 * A298043  If n = Sum_{i=1..h} 2^b_i with b_1 > ... > b_h >= 0, then a(n) =
 *          Sum_{i=1..h} i * 2^b_i.
 * https://oeis.org/A298043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = Σ_{i=1..h} i·2^b_i where n = Σ 2^b_i, with b_1>…>b_h≥0.
 *            (equivalently, for each set bit of n from most significant to
 *            least, add (position index)*2^bit position.)
 * Range:     u8 n <= 179, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* needed for uint64_t */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* a(0)=0 */

    seq_t sum = 0;
    idx_t msb = 0;                     /* number of bits in n */
    idx_t tmp = n;
    while (tmp > 0) { tmp >>= 1; msb++; }

    idx_t pos = 0;                     /* current index i (1-based) */
    for (idx_t exp = msb; exp-- > 0; ) /* exp from msb-1 down to 0 */
    {
        if (n & ((idx_t)1 << exp))
        {
            pos++;                                 /* increment position */
            seq_t pow2 = (seq_t)((idx_t)1 << exp);  /* 2^exp in seq_t */
            sum += (seq_t)pos * pow2;              /* add i*2^exp */
        }
    }
    return sum;
}
