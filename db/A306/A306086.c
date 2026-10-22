/*
 * A306086  Primes which remain prime after all odd digits are deleted.
 * https://oeis.org/A306086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th prime whose decimal digits consist of exactly
 *            one ‘2’, any number of zeros before that ‘2’, no zeros after,
 *            and all other digits odd
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   in_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/in_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 */
    if (n == 1) return 2;          /* first term */

    idx_t count = 1;               /* we already have the prime 2 */
    seq_t cand = 3;                /* start checking odd numbers */

    while (true) {
        if (!is_prime_big(cand)) {    /* skip composites */
            cand = cand + 2;
            continue;
        }
        if (in_seq(cand)) {
            ++count;
            if (count == n) return cand;
        }
        cand = cand + 2;
    }
}
