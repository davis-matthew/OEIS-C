/*
 * A265885  a(n) = n IMPL prime(n), where IMPL is the bitwise logical
 *          implication.
 * https://oeis.org/A265885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n IMPL prime(n) (bitwise logical implication)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, get_prime, mask_for_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/mask_for_len.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; n==0 shouldn't happen, but guard anyway */
        return 0;

    uint32_t p32 = get_prime(n);     /* prime(n) is always < 2^32 for n <= 10⁵ */
    seq_t p   = (seq_t)p32;
    unsigned int len = bitlen_seq_t(p);

    seq_t mask = mask_for_len(len);

    seq_t nn = (seq_t)n;
    seq_t res = ((~nn) | p) & mask;
    return res;
}
