/*
 * A367056  G.f. satisfies A(x) = 1 + x*A(x)^2 + x^3*A(x).
 * https://oeis.org/A367056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(n-2*k+1,k) *
 *            binomial(2*n-5*k,n-3*k) / (n-2*k+1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (also satisfies a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=17, a(5)=52) */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k); */
#include "../utils/seq_mul.h"    /* provides seq_t seq_mul(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static const seq_t base_vals[6] = {1, 1, 2, 6, 17, 52};

    if (n < cache_sz)          /* value already computed */
        return cache[n];

    /* expand cache to hold up to n */
    cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));

    for (idx_t i = cache_sz; i <= n; ++i) {
        seq_t val;
        if (i < 6) {
            val = base_vals[(size_t)i];
        } else {
            idx_t maxk = i / 3;
            seq_t sum = 0;
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t part1_n = i - 2 * k + 1;
                seq_t c1 = binomial(part1_n, k);
                idx_t part2_n = 2 * i - 5 * k;
                seq_t c2 = binomial(part2_n, i - 3 * k);
                seq_t prod = seq_mul(c1, c2);
                prod = prod / (seq_t)part1_n;   /* integer division */
                sum += prod;
            }
            val = sum;
        }
        cache[i] = val;
    }
    cache_sz = n + 1;
    return cache[n];
}
