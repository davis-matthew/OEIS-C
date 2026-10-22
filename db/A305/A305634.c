/*
 * A305634  Even numbers that are not perfect powers.
 * https://oeis.org/A305634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = even numbers that are NOT perfect powers (m^k, k>=2)
 * Range:     u8 n <= 26, u16 n <= 968, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/floor_sqrt.h"

static bool is_perfect_power(seq_t x)
{
    /* A perfect power m^k with k>=2.  Only need to test for even x. */
    if (x < 4) return false;      /* 2,3 are not perfect powers with k>=2 */

    /* Limit for base m: floor(sqrt(x)) */
    seq_t limit = floor_sqrt(x);

    for (idx_t m = 2; m <= (idx_t)limit; ++m) {
        seq_t base = (seq_t)m;
        seq_t power = seq_mul(base, base);   /* m^2 */
        if (power == x) return true;
        while (power < x) {                   /* grow exponent */
            power = seq_mul(power, base);     /* power *= m */
            if (power == x) return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, but return 0 for safety */

    seq_t candidate = 2;       /* first even number */
    idx_t count = 0;

    for (;;) {
        if (!is_perfect_power(candidate)) {
            ++count;
            if (count == n) return candidate;
        }
        candidate += 2;        /* next even number */
    }
}
