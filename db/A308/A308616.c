/*
 * A308616  Number of well-formed formulas of length n in a formal
 *          propositional language with one unitary operator, one binary
 *          operator, and one propositional variable.
 * https://oeis.org/A308616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{4b+3u+1=n} binomial(2b+u, u) * Catalan(b)
 * Range:     u8 n <= 20, u16 n <= 33, u32 n <= 58, u64 n <= 106,
 *            u128 n <= 200, big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t, idx_t); */
#include "../utils/catalan.h"    /* provides seq_t catalan(idx_t); */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* sequence is defined only for n >= 1 */
    seq_t sum = 0;
    idx_t max_b = (n - 1) / 4;          /* b satisfies 4b ≤ n-1 */
    for (idx_t b = 0; b <= max_b; ++b) {
        idx_t rem = n - 1 - 4 * b;
        if (rem % 3 != 0) continue;    /* u must be integer */
        idx_t u = rem / 3;
        seq_t bin = binomial(2 * b + u, u);
        seq_t cat = catalan(b);
        sum += bin * cat;
    }
    return sum;
}
