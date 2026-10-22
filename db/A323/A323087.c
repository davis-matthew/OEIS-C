/*
 * A323087  Number of strict factorizations of n into factors > 1 such that no
 *          factor is a power of any other factor.
 * https://oeis.org/A323087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count strict factorizations of n into factors > 1 such that no
 *            factor is a power of any other factor. (Enumeration by recursion
 *            with increasing factors.)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static bool is_power_of(idx_t base, idx_t target)
{
    if (base <= 1)                 /* 1 is never considered a power */
        return false;
    if (target < base * base)      /* exponent 2 is the smallest possible */
        return false;

    idx_t p = base * base;
    while (p <= target) {
        if (p == target)
            return true;
        /* avoid overflow in multiplication */
        if (p > UINT64_MAX / base)
            break;
        p *= base;
    }
    return false;
}

/* Recursive enumeration of factorizations.
   m      : current product that still has to be factored
   minf   : smallest factor allowed (to keep factors strictly increasing)
   stack  : array of factors chosen so far
   depth  : number of elements currently in stack
   Returns the number of valid factorizations of m. */
static seq_t _enum_factorizations(idx_t m, idx_t minf,
                                  uint64_t *stack, size_t depth)
{
    seq_t count = 0;

    /* Count the factorization that ends with the factor m itself,
       if it does not violate the power condition with any previous factor. */
    if (m >= minf) {
        bool ok = true;
        for (size_t i = 0; i < depth; ++i) {
            if (is_power_of(stack[i], m) || is_power_of(m, stack[i])) {
                ok = false;
                break;
            }
        }
        if (ok)
            count += 1;
    }

    for (idx_t d = minf; d * d <= m; ++d) {
        if (m % d != 0)
            continue;

        idx_t other = m / d;
        if (other < d)               /* maintain strictly increasing order */
            continue;

        /* Ensure d does not share a power relation with any previous factor */
        bool ok = true;
        for (size_t i = 0; i < depth; ++i) {
            if (is_power_of(stack[i], d) || is_power_of(d, stack[i])) {
                ok = false;
                break;
            }
        }
        if (!ok)
            continue;

        stack[depth] = (uint64_t)d;
        count += _enum_factorizations(other, d + 1, stack, depth + 1);
    }

    return count;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; a(0) is undefined */
        return 0;
    if (n == 1)                /* a(1) = 1 by definition */
        return 1;

    /* Recursion buffer; depth never exceeds 32 for n <= 10000 */
    uint64_t stack[32];
    return _enum_factorizations(n, 2, stack, 0);
}
