/*
 * A346006  Successive numbers arising from the Moessner construction of the
 *          sequence of fourth powers on page 64 of Conway-Guy's "Book of
 *          Numbers".
 * https://oeis.org/A346006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(4,i+1)*((n+i)/4)^(i+1) where i is chosen so that
 *            n ≡ -i (mod 4) for 0 ≤ i < 4
 * Range:     u8 n <= 12, u16 n <= 60, u32 n <= 1020, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    uint8_t mod = n % 4;
    uint8_t i;   /* 0 ≤ i < 4 */
    switch (mod) {
        case 0: i = 0; break;   /* n ≡ 0 (mod 4) → i = 0 */
        case 1: i = 3; break;   /* n ≡ 1 (mod 4) → i = 3 */
        case 2: i = 2; break;   /* n ≡ 2 (mod 4) → i = 2 */
        case 3: i = 1; break;   /* n ≡ 3 (mod 4) → i = 1 */
        default: i = 0; /* unreachable */ break;
    }

    idx_t t = (n + i) / 4;            /* integer division is exact */
    seq_t base = (seq_t)t;
    seq_t pow  = big_pow(base, (seq_t)(i + 1));
    seq_t bin  = binomial(4, i + 1);
    return bin * pow;
}
