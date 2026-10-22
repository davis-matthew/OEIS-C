/*
 * A337834  If k is the n-th number ending in 1,3,7, or 9, a(n) is the least
 *          prime > k ending in k.
 * https://oeis.org/A337834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 402, u32 n <= 520, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For the n‑th integer k ending in 1,3,7, or 9, find the
   least prime p > k that ends with the digits of k.  The prime p is
   the smallest number in the arithmetic progression k, k+10^d, k+2·10^d,
   … that is prime, where d = digit_len(k). */

#include <stdbool.h>
#include "../utils/digit_len.h"   /* int digit_len(seq_t x); */
#include "../utils/pow10.h"      /* seq_t pow10(idx_t e); */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined for offset 0, return 0 */

    /* Find the n‑th integer ending in 1,3,7,9 */
    idx_t q = (n - 1) / 4;           /* block number */
    idx_t r = (n - 1) % 4;           /* position within the block */
    uint8_t digit = (r == 0) ? 1
                 : (r == 1) ? 3
                 : (r == 2) ? 7
                 : 9;

    /* Compute k = 10·q + digit, cast to seq_t */
    seq_t k = (seq_t)(10 * (seq_t)q + (seq_t)digit);

    /* Determine number of decimal digits of k */
    int d = digit_len(k);

    /* Modulus for the arithmetic progression: 10^d */
    seq_t mod = pow10(d);

    /* First candidate > k in the progression */
    seq_t cand = k + mod;

    /* Search for the least prime in this progression */
    while (!is_prime_big(cand)) {
        cand += mod;
    }

    return cand;
}
