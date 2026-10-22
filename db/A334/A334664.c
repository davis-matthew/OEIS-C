/*
 * A334664  a(n) = Product_{d|n} gcd(d, tau(d)).
 * https://oeis.org/A334664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{d|n} gcd(d, τ(d))
 * Range:     u8 n <= 23, u16 n <= 71, u32 n <= 359, u64 n <= 1439,
 *            u128 n <= 5039, big any n
 * Helpers:   divisor_count_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"

/* Computes the OEIS sequence A334664.
   For n ≥ 1 it returns the product over all divisors d of n
   of gcd(d, τ(d)), where τ(d) is the number of divisors of d. */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset starts at 1; undefined below */
        return 0;

    seq_t res = 1;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor i */
            seq_t di   = (seq_t)i;
            seq_t taui = (seq_t)divisor_count_u64(i);
            res *= gcd(di, taui);

            /* complementary divisor */
            idx_t j = n / i;
            if (j != i) {
                seq_t dj   = (seq_t)j;
                seq_t tauj = (seq_t)divisor_count_u64(j);
                res *= gcd(dj, tauj);
            }
        }
    }
    return res;
}
