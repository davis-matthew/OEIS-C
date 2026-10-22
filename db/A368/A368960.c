/*
 * A368960  a(n) is the least prime of the form k*n + (greatest square < k*n).
 * https://oeis.org/A368960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) is the least prime of the form k*n + t² * where t is the
 *            largest integer with t² < k*n. * Computation by a
 *            straightforward search over k
 * Range:     u8 n <= 7, u16 n <= 815, u32 n <= 2179, u64 n <= 9682,
 *            u128 n <= 9682, big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Search for the smallest prime of the described form. */
    for (idx_t k = 1;; ++k) {
        /* m = k * n */
        seq_t m = (seq_t)k * n;

        /* t = floor(sqrt(m)), adjusted if m is a perfect square */
        seq_t t = floor_sqrt(m);
        if (t * t == m) {
            --t;                         /* greatest square < m */
        }

        seq_t t2 = t * t;               /* t² */

        seq_t candidate = m + t2;       /* k*n + t² */

        /* Skip trivial non‑prime candidates */
        if (candidate <= 1) {
            continue;
        }

        if (is_prime_big(candidate)) {
            return candidate;           /* found the least prime */
        }
    }
}
