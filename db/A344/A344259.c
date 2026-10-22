/*
 * A344259  For any number n with binary expansion (b(1), ..., b(k)), the
 *          binary expansion of a(n) is (b(1), ..., b(ceiling(k/2))).
 * https://oeis.org/A344259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = binary(n) truncated to the first ceil(bitlen(n)/2) bits *
 *            (i.e. a(n) = n >> floor(bitlen(n)/2), with a(0)=0)
 * Range:     u8 n <= 573, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the number of bits needed to represent n (without leading zeros).
   For n==0, return 1 to match the OEIS definition of a(0)=0. */
static unsigned bitlen_idx_t(idx_t n)
{
    if (n == 0)
        return 1;
    unsigned len = 0;
    while (n) {
        len++;
        n >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    unsigned len = bitlen_idx_t(n);
    unsigned shift = len / 2;            /* floor(k/2)  */
    return ((seq_t)n) >> shift;          /* keep the top ceil(k/2) bits */
}
