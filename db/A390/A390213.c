/*
 * A390213  Number of divisors d of n such that d^(n/d) == d (mod (n/d)).
 * https://oeis.org/A390213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ d | d divides n, d^(n/d) ≡ d (mod n/d) }
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mod_pow.h"

static inline bool test_divisor(idx_t d, idx_t m)
{
    seq_t base = (seq_t)d;
    seq_t mod = (seq_t)m;
    seq_t val = mod_pow(base, m, mod);          /* d^m mod m */
    seq_t rem = (seq_t)(d % m);                /* d mod m  */
    return val == rem;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* below offset */
    seq_t cnt = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t m1 = n / i;
            if (test_divisor(d1, m1))
                ++cnt;
            if (i != m1) {
                idx_t d2 = m1;
                idx_t m2 = i;
                if (test_divisor(d2, m2))
                    ++cnt;
            }
        }
    }
    return cnt;
}
