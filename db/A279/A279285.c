/*
 * A279285  Self-composition of the Pell numbers; g.f.: A(x) = G(G(x)), where
 *          G(x) = g.f. of A000129.
 * https://oeis.org/A279285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) -5*a(n-2) -6*a(n-3) - a(n-4); initial values:
 *            a(0)=0,a(1)=1,a(2)=4,a(3)=18
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t cache[10001] = { (seq_t)0, (seq_t)1, (seq_t)4, (seq_t)18 };
static idx_t last_computed = 3;

seq_t a(idx_t n)
{
    if (n <= last_computed)
        return cache[n];
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = cache[i-1] * (seq_t)6
                 - cache[i-2] * (seq_t)5
                 - cache[i-3] * (seq_t)6
                 - cache[i-4];
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
