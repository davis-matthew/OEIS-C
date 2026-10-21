/*
 * A271497  If n is a multiple of 3 then a(n) = n/3, otherwise a(n) = n'+n'',
 *          where n' (resp. n'') is obtained by sorting the bits of the binary
 *          expansion of n into increasing (resp. decreasing) order.
 * https://oeis.org/A271497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n/3 if 3|n; otherwise a(n) = n' + n''
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _ones_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_ones_len.h"

seq_t a(idx_t n)
{
    /* 3|n ? (n/3) : n' + n''  */
    if ((n % 3) == 0) return (seq_t)(n / 3u);

    /* population count (number of 1-bits) */
    idx_t k = 0;
    idx_t m = n;
    while (m) {
        k += m & 1u;
        m >>= 1u;
    }

    /* bit length (position of highest set bit + 1) */
    idx_t L = 0;
    m = n;
    while (m) {
        m >>= 1u;
        ++L;
    }

    /* n' = (1 << k) - 1  -> low k bits are 1 */
    seq_t np = _ones_len(k);

    /* n'' = ((1 << L) - 1) - ((1 << (L - k)) - 1)
           -> high k bits are 1, low (L - k) bits are 0  */
    seq_t allL   = _ones_len(L);
    seq_t allLk  = _ones_len(L - k);
    seq_t npp    = allL - allLk;

    return np + npp;
}
