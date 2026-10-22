/*
 * A341896  a(n) is the number of words of length n over the alphabet {a,b,c}
 *          with an even number of appearances of the letter 'a' and the sum
 *          of appearances of the letters 'b' and 'c' add up to at most 3.
 * https://oeis.org/A341896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0..3} [ (n-j) is even ] * C(n,j) * 2^j (C(n,j) is
 *            the binomial coefficient)
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: binomial coefficient for j = 0..3, using seq_t arithmetic */
static inline seq_t binom_small(idx_t n, idx_t j)
{
    seq_t result = 1;
    if (j == 0) return 1;
    if (j == 1) return (seq_t)n;
    if (j == 2) return (seq_t)n * ((seq_t)n - 1) / 2;
    /* j == 3 */
    return (seq_t)n * ((seq_t)n - 1) * ((seq_t)n - 2) / 6;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 0; j <= 3 && j <= n; ++j) {
        /* n-j must be even */
        if (((n - j) & 1u) == 0) {
            seq_t comb = binom_small(n, j);
            seq_t add = comb << j; /* multiply by 2^j */
            sum += add;
        }
    }
    return sum;
}
