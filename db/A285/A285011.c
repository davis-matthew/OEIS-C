/*
 * A285011  Numbers with digits 7 and 9 only.
 * https://oeis.org/A285011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   _length_of_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A285011 : Numbers whose decimal digits are only 7 and 9.
    The n-th term is obtained by writing (n-((1<<L)-1)) in binary with
    exactly L bits (L = ⌈log₂(n+2)⌉−1) and mapping 0→7, 1→9.  The
    terms increase numerically, so this generates the sequence in
    ascending order.  */

#include <stdint.h>   /* for uint64_t */
#include <stddef.h>
#include "../utils/_length_of_n.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n<1, return 0 */

    const idx_t L = _length_of_n(n);                       /* digit length */
    const idx_t offset = n - (((idx_t)1 << L) - 1);         /* 0‑based index within length L */

    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < L; ++i) {
        /* process bits from most significant to least */
        const uint8_t bit = (uint8_t)((offset >> (L - 1 - i)) & 1U);
        result = result * (seq_t)10 + (seq_t)(bit ? 9 : 7);
    }
    return result;
}
