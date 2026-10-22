/*
 * A353525  a(n) = 1 if the number of trailing zeros in primorial base
 *          representation of n is odd, otherwise 0.
 * https://oeis.org/A353525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if the index of the least prime not dividing n is even,
 *            otherwise 0. (Least prime index means 2 has index 1, 3 has index
 *            2, etc.)
 * Range:     u8 n <= 9763, u16 n <= 9763, u32 n <= 9763, u64 n <= 9763,
 *            u128 n <= 9763, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;                  /* offset starts at 1 */

    uint64_t m = (uint64_t)n;              /* use 64‑bit arithmetic for n */
    idx_t k = 1;                           /* prime index (1‑based) */

    while (1) {
        uint32_t p = get_prime(k);         /* k‑th prime, fits in 32 bits */
        if (m % p != 0) {                  /* found least prime not dividing n */
            return (seq_t)((k & 1) ? 0 : 1); /* even index → 1, odd → 0 */
        }
        k++;                               /* try next prime */
    }
}
