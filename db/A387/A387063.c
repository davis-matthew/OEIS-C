/*
 * A387063  G.f. A(x) satisfies A(x) = 1 + x/(1-x^2)^2 * A(x)^2.
 * https://oeis.org/A387063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(2*n-3*k-1, k) * Catalan(n-2*k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A387063;  offset 0  (a(0)=1) */

#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) -> seq_t */
#include "../utils/catalan.h"    /* catalan(idx_t k) -> seq_t */

seq_t a(idx_t n)
{
    /* a(0) is special: the binomial argument would be negative */
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    idx_t max_k = n / 2;
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t upper = 2 * n - 3 * k - 1;
        /* binomial will return 0 if upper < k, but guard for clarity */
        if (k > 0 && upper < k) continue;
        seq_t bin = binomial(upper, k);
        seq_t cat = catalan(n - 2 * k);
        sum += bin * cat;
    }
    return sum;
}
