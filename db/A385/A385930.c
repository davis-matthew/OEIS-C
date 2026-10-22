/*
 * A385930  Minimum base in which n achieves its maximum multiplicative
 *          persistence.
 * https://oeis.org/A385930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching bases 2..n for the maximum multiplicative persistence */
#include <stdint.h>
#include <stdbool.h>

/* Product of the digits of x in the given base */
static inline seq_t digit_product(seq_t x, seq_t base)
{
    seq_t prod = 1;
    while (x) {
        seq_t d = x % base;
        prod *= d;
        if (!prod)          /* zero digit: product remains zero */
            break;
        x /= base;
    }
    return prod;
}

/* Multiplicative persistence of x in base b */
static inline idx_t persistence(seq_t x, seq_t base)
{
    idx_t steps = 0;
    while (x >= base) {
        seq_t prod = digit_product(x, base);
        x = prod;
        ++steps;
    }
    return steps;
}

/* sequence term a(n) : smallest base that gives maximal persistence of n */
seq_t a(idx_t n)
{
    seq_t best_base = 2;
    idx_t best_steps = 0;              /* no sentinel needed */

    for (idx_t base = 2; base <= n; ++base) {
        idx_t steps = persistence((seq_t)n, (seq_t)base);
        if (steps > best_steps || (steps == best_steps && base < (idx_t)best_base)) {
            best_steps = steps;
            best_base  = (seq_t)base;
        }
    }
    return best_base;
}
