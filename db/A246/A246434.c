/*
 * A246434  Expansion of (3*x/2 - 1 - (7*x - 2)/(2*sqrt(1 - 4*x)))/x.
 * https://oeis.org/A246434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 33, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A246434
   Formula: a(n) = binomial(2*n-1, n+1) - catalan(n)
   (for n ≥ 1; a(0) is undefined, returns 0)
   This implementation uses an overflow‑free multiplicative computation
   of binomial coefficients and Catalan numbers.  It works for any
   integer type seq_t (signed or arbitrary precision) and any idx_t
   (uint64_t). */

#include <stddef.h>
#include <stdint.h>

/* Helper: Euclidean gcd for unsigned long long (used for reduction). */
static inline unsigned long long gcd_ull(unsigned long long a, unsigned long long b)
{
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Computes binomial(a, b) for non‑negative arguments,
   avoiding overflow by reducing at each step. */
static inline seq_t binomial_unsigned(idx_t a, idx_t b)
{
    if (b > a) return (seq_t)0;
    /* Use symmetry to minimise loop iterations. */
    if (b > a - b) b = a - b;

    seq_t result = 1;
    for (idx_t i = 1; i <= b; ++i) {
        unsigned long long num = a - i + 1;
        unsigned long long den = i;
        unsigned long long g = gcd_ull(num, den);
        num /= g;
        den /= g;

        /* Divide first – guarantees integer division. */
        result /= (seq_t)den;
        result *= (seq_t)num;
    }
    return result;
}

/* Catalan number C(n) = binomial(2n, n)/(n+1). */
static inline seq_t catalan_value(idx_t n)
{
    seq_t bin = binomial_unsigned(2 * n, n);
    return bin / (seq_t)(n + 1);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined offset */

    seq_t bin = binomial_unsigned(2 * n - 1, n + 1);
    seq_t cat = catalan_value(n);
    return bin - cat;
}
