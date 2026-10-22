/*
 * A349785  a(n) = 1 + Product_{k=1..n-1} a(gcd(n,k)).
 * https://oeis.org/A349785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 1 + Product_{d|n, d < n} a(d)^phi(n/d)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 15, u64 n <= 23, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow, calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    if (n == 0) return 0;   /* undefined, but no call */

    if (n > cap) {
        size_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* out of memory */
        for (size_t i = cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cap   = new_cap;
    }

    if (cache[n - 1] != 0) return cache[n - 1];

    if (n == 1) {
        cache[0] = 2;
        return cache[0];
    }

    seq_t prod = 1;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            if (d1 < n) {
                seq_t val = (d1 == 1) ? 2 : cache[d1 - 1];
                if (d1 > 1 && val == 0) val = a(d1);
                seq_t phi = calc_phi((seq_t)(n / d1));
                seq_t pow = big_pow(val, phi);
                prod *= pow;
            }

            idx_t d2 = n / i;
            if (d2 != d1 && d2 < n) {
                seq_t val = (d2 == 1) ? 2 : cache[d2 - 1];
                if (d2 > 1 && val == 0) val = a(d2);
                seq_t phi = calc_phi((seq_t)(n / d2));
                seq_t pow = big_pow(val, phi);
                prod *= pow;
            }
        }
    }

    seq_t result = prod + 1;
    cache[n - 1] = result;
    return result;
}
