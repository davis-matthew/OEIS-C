/*
 * A362227  a(n) = Product_{k=1..w(n)} p(k)^(S(n,k)-1), where set S(n,k) = row
 *          n of A272011 and w(n) = A000120(n) is the binary weight of n.
 * https://oeis.org/A362227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=1}^{w(n)} prime(k)^{(position_k+1)} where the set
 *            bits of n are examined from most to least significant. The k‑th
 *            set bit (k=1 for the most significant 1) is at position
 *            position_k (0‑based from the LSB), and its contribution is
 *            prime(k) raised to (position_k+1). This yields the value...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 58, u64 n <= 238,
 *            u128 n <= 1018, big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Empty product for n = 0 */
    if (n == 0)
        return (seq_t)1;

    seq_t prod = (seq_t)1;
    idx_t rank = 0;                 /* 1‑based index of the set bit */

    /* Scan bits from most significant to least significant */
    for (int pos = 63; pos >= 0; --pos) {
        if ((n >> pos) & ((idx_t)1)) {
            ++rank;                                 /* prime index */
            seq_t p   = (seq_t)get_prime(rank);     /* prime(k) */
            seq_t exp = (seq_t)(pos + 1);            /* exponent = position+1 */
            seq_t mult = big_pow(p, exp);            /* p^exp */
            prod *= mult;
        }
    }
    return prod;
}
