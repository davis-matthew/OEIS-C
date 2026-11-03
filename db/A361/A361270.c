/*
 * A361270  Number of 1324-avoiding odd Grassmannian permutations of size n.
 * https://oeis.org/A361270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = { if n is even: C((n-2)/2 + 3, 3) + C((n-4)/2 + 3, 3) +
 *            2*C((n-6)/2 + 3, 3) * if n is odd : 2*C((n-3)/2 + 3, 3) * where
 *            C(x,3) = (x+3)(x+2)(x+1)/6 for integer x ≥ 0
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static seq_t binom_n3(idx_t k) {
    /* Compute (k+3 choose 3) = (k+3)(k+2)(k+1)/6
       using 64‑bit intermediates to avoid overflow.      */
    unsigned long long a = (unsigned long long)k + 3ULL;
    unsigned long long b = (unsigned long long)k + 2ULL;
    unsigned long long c = (unsigned long long)k + 1ULL;
    unsigned long long prod = a * b * c;      /* division is exact */
    prod /= 6ULL;
    return (seq_t)prod;
}

seq_t a(idx_t n)
{
    /* Offset 0: a(0) = 0, a(1) = 0 */
    if (n < 2) return 0;

    if ((n & 1ULL) == 0ULL) {
        /* n is even */
        seq_t sum = 0;

        /* term d = 2 */
        if (n >= 2) {
            idx_t k = (n - 2) / 2;
            sum += binom_n3(k);
        }

        /* term d = 4 */
        if (n >= 4) {
            idx_t k = (n - 4) / 2;
            sum += binom_n3(k);
        }

        /* term d = 6 */
        if (n >= 6) {
            idx_t k = (n - 6) / 2;
            sum += 2 * binom_n3(k);
        }

        return sum;
    } else {
        /* n is odd */
        if (n < 3) return 0;
        idx_t k = (n - 3) / 2;
        return 2 * binom_n3(k);
    }
}
