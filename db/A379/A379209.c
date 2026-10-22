/*
 * A379209  G.f. A(x) satisfies A(x) = 1/((1 - x*A(x)^2) * (1 - x*A(x))).
 * https://oeis.org/A379209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A379209
   Formula: a(n) = Sum_{k=0..n} binomial(n+2*k+1,k) * binomial(2*n,n-k)/(n+2*k+1) */

/* required integer types are supplied by the build */
#include "../utils/binom.h" /* seq_t binom(idx_t a, idx_t b) */
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b) */

/* multiply two seq_t values and divide by d, reducing with gcds first
   to keep intermediate values small.  Works for both built‑in integer
   types and the arbitrary‑precision seq_t. */
static inline seq_t mul_div_exact(seq_t x, seq_t y, seq_t d)
{
    seq_t g = gcd(x, d);
    if (g > 1) { x /= g; d /= g; }
    g = gcd(y, d);
    if (g > 1) { y /= g; d /= g; }
    /* after the reductions the division is exact */
    return (x * y) / d;
}

/* compute a(n) for the sequence.  The calculation is performed
   sequentially for k = 0 .. n.  Even for large n the use of
   binom() (which uses only + - * /) keeps the implementation
   simple and works with every supported seq_t type. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin1 = binom(n + 2*k + 1, k);      /* C(n+2k+1, k)   */
        seq_t bin2 = binom(2*n, n - k);          /* C(2n, n-k)     */
        seq_t denom = (seq_t)(n + 2*k + 1);
        seq_t term = mul_div_exact(bin1, bin2, denom);
        sum += term;
    }
    return sum;
}
