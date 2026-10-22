/*
 * A332075  Odd numbers 2n+1 such that k + 2^m is prime, where k and m are the
 *          odd part and 2-valuation, respectively, of 2n.
 * https://oeis.org/A332075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 6000, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A332075
   a(n) = the n‑th odd integer 2k+1 such that (odd part of 2k) + 2^v2(2k) is prime.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for n=0, return 0 */

    idx_t found = 0;                        /* number of sequence terms seen */
    idx_t k     = 1;                        /* current k corresponding to 2k+1 */

    while (found < n) {
        /* current candidate odd number 2k+1 */
        seq_t candidate = (seq_t)2 * (seq_t)k + 1;

        /* compute v2(2k) = 1 + v2(k) */
        unsigned int v2k = __builtin_ctzll((unsigned long long)k);
        unsigned int exp = v2k + 1;          /* exponent of 2 in 2k */

        /* odd part of 2k = k >> v2k */
        seq_t odd_part = (seq_t)(k >> v2k);

        /* compute sum = odd_part + 2^exp */
        seq_t sum = odd_part + ((seq_t)1 << exp);

        if (is_prime_big(sum)) {
            /* this candidate is part of the sequence */
            found++;
            if (found == n) {
                return candidate;
            }
        }
        k++;
    }
    return 0;   /* unreachable */
}
