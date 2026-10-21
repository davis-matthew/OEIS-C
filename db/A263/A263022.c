/*
 * A263022  a(n) = gcd(n, 1^(n-1) + 2^(n-1) + ... + (n-1)^(n-1)) for n > 1.
 * https://oeis.org/A263022
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, Σ_{k=1}^{n-1} k^(n-1)) (n > 1)
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* The definition is only for n > 1; for safety return 0 otherwise */
    if (n <= 1)
        return 0;

    seq_t mod = (seq_t)n;
    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k) {
        seq_t term = mod_pow((seq_t)k, n - 1, mod);
        sum += term;
        if (sum >= mod)
            sum -= mod;   /* keep sum < mod */
    }
    return gcd(mod, sum);
}
