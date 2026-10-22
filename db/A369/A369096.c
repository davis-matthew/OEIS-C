/*
 * A369096  Numbers k >= 2 such that omega(k) divides the sum of the
 *          triangular numbers T(k) and T(k+1), where omega(k) is the number
 *          of distinct primes dividing k (A001221).
 * https://oeis.org/A369096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer k ≥ 2 such that ω(k) | (k+1)² where ω(k)
 *            is the number of distinct prime factors of k (A001221)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* a(1) = 2, n is 1‑based */
    if (n == 0) return (seq_t)0;          /* undefined, harmless fallback */

    idx_t k = 2;                           /* current candidate k */
    idx_t found = 0;                       /* number of terms found so far */

    while (true) {
        int omega = count_distinct_primes((uint64_t)k);
        if (omega > 0) {
            int kplus = (int)((k + 1) % (idx_t)omega);
            int sqmod = (kplus * kplus) % omega;
            if (sqmod == 0) {
                found++;
                if (found == n) {
                    return (seq_t)k;      /* cast k to seq_t */
                }
            }
        }
        ++k;
    }
}
