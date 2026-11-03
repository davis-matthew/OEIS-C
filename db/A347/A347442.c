/*
 * A347442  Number of factorizations of n with integer reverse-alternating
 *          product.
 * https://oeis.org/A347442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts weakly increasing factorizations of n into integers
 *            >1 whose reverse alternating product is an integer
 * Range:     u8 n <= 9215, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 124 terms through n = 9258 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Recursive helper that counts the admissible factorizations of 'n'.
   'min_factor' enforces nondecreasing order.
   'stack' holds the current partial factorization.
   'depth' is the current length of the stack. */
static seq_t
count_adjusted(idx_t n, idx_t min_factor, idx_t stack[], size_t depth)
{
    seq_t total = 0;

    /* Try all divisors f of n that are >= min_factor and <= sqrt(n). */
    for (idx_t f = min_factor; f * f <= n; ++f) {
        if (n % f == 0) {
            stack[depth] = f;
            total += count_adjusted(n / f, f, stack, depth + 1);
        }
    }

    /* The last factor may be n itself (if n >= min_factor). */
    if (n >= min_factor) {
        stack[depth] = n;
        size_t len = depth + 1;
        idx_t numerator   = 1;
        idx_t denominator = 1;
        idx_t parity = (idx_t)(len & 1);          /* 1 if len odd, else 0 */
        for (idx_t i = 0; i < len; ++i) {
            idx_t pos = i + 1;                     /* 1‑based position */
            if ((pos & 1) == parity)
                numerator *= stack[i];
            else
                denominator *= stack[i];
        }
        if (denominator != 0 && numerator % denominator == 0)
            total += (seq_t)1;
    }

    return total;
}

/* Public function required by the build system. */
seq_t
a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;           /* a(1) = 1 by definition */

    /* A stack big enough for the maximal number of factors (log₂(10⁴) < 14). */
    idx_t stack[64];

    return count_adjusted(n, (idx_t)2, stack, (size_t)0);
}
