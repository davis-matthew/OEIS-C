/*
 * A342121  a(n) is the remainder when the larger of n and its binary reverse
 *          is divided by the smaller.
 * https://oeis.org/A342121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = max(n, A030101(n)) mod min(n, A030101(n))
 * Range:     u8 n <= 517, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>        /* for uint64_t used by idx_t */
#include <stddef.h>       /* for size_t */

/* Compute the binary reverse of n (no leading zeros) */
static inline seq_t reverse_bits(idx_t n)
{
    idx_t rev = 0;
    while (n != 0) {
        rev = (rev << 1) | (n & 1);
        n >>= 1;
    }
    return (seq_t)rev;
}

seq_t a(idx_t n)
{
    seq_t rev = reverse_bits(n);
    seq_t big = rev > (seq_t)n ? rev : (seq_t)n;
    seq_t small = rev > (seq_t)n ? (seq_t)n : rev;
    return big % small;
}
