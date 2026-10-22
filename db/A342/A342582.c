/*
 * A342582  a(n) is the least multiple of n that is a "binary antipalindrome"
 *          (i.e., an element of A035928).
 * https://oeis.org/A342582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n*A318569(n) (least multiple of n that is a binary
 *            antipalindrome)
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 764, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"   /* provides unsigned bitlen_seq_t(seq_t x) */

/* Return true if x is a binary antipalindrome.
   An antipalindrome has even bit length and each symmetric pair of bits are different. */
static bool is_antipal(seq_t x)
{
    /* n = 0 cannot be an antipalindrome */
    if (x == 0)
        return false;

    unsigned len = bitlen_seq_t(x);
    if (len & 1)      /* odd length cannot be a (binary) antipalindrome */
        return false;

    for (idx_t i = 0; i < (idx_t)(len >> 1); ++i)
    {
        bool bit_low  = (((x >> i)        & 1) != 0);
        bool bit_high = (((x >> (len - 1 - i)) & 1) != 0);
        if (bit_low == bit_high)
            return false;
    }
    return true;
}

/* Least multiple of n that is a binary antipalindrome. */
seq_t a(idx_t n)
{
    /* Even n: try all multiples.  Odd n: multiples are always odd, so
       skip the first odd multiple (1*n is odd) and step by 2. */
    idx_t k = (n & 1) ? 2 : 1;                 /* first multiplier */
    for (;; k += (n & 1) ? 2 : 1)              /* step size depends on parity of n */
    {
        seq_t x = (seq_t)n * k;
        if (is_antipal(x))
            return x;
    }
}
