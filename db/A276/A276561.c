/*
 * A276561  For n-th odd prime prime(n) in binary form, a(n) is the decimal
 *          value of the bits in between the most significant and least
 *          significant bits which are both 1. Since there are no middle bits
 *          for odd_prime(1) = 3 = (11)_2, a(1) = 0.
 * https://oeis.org/A276561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A164089(prime(n+1)) (remove the most significant * and
 *            least significant 1-bits from the binary representation of the *
 *            n‑th odd prime)
 * Range:     u8 n <= 54, u16 n <= 113, u32 n <= 113, u64 n <= 113,
 *            u128 n <= 113, big any n
 * Helpers:   bitlen_seq_t, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* n ≥ 1; odd prime = prime(n+1) since prime(1)=2. */
    uint32_t p32 = get_prime(n + 1);
    seq_t p = (seq_t)p32;

    unsigned bl = bitlen_seq_t(p);
    if (bl <= 2)
        return 0;                     /* no middle bits */

    seq_t mask = ((seq_t)1 << (bl - 2)) - 1;
    return (p >> 1) & mask;           /* drop MSB and LSB */
}
