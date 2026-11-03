/*
 * A331023  Numerator of number of factorizations of n divided by number of
 *          strict factorizations of n A001055(n)/A045778(n).
 * https://oeis.org/A331023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Numerator of A001055(n)/A045778(n)
 * Range:     u8 n <= 2879, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 7409 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Recursive count of multiplicative factorizations of n
   with all factors >= min_factor (nondecreasing order) */
static seq_t factcount(uint64_t n, uint64_t min_factor)
{
    if (n == 1) return 1;                 /* trivial factorization */
    seq_t count = 1;                      /* the factorization consisting of the single factor n itself */
    for (uint64_t i = min_factor; i <= n / i; ++i) {
        if (n % i == 0) {
            count += factcount(n / i, i);
        }
    }
    return count;
}

/* Recursive count of strict multiplicative factorizations of n
   with all factors >= min_factor (strictly increasing order) */
static seq_t strictfactcount(uint64_t n, uint64_t min_factor)
{
    if (n == 1) return 1;
    seq_t count = 1;                      /* the factorization consisting of n itself */
    for (uint64_t i = min_factor; i <= n / i; ++i) {
        if (n % i == 0 && i < (n / i)) {  /* distinct factors, strictly increasing */
            count += strictfactcount(n / i, i + 1);
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1 (offset 1). */
    if (n < 1) return 0;                  /* undefined for n = 0, but this case will not occur */

    seq_t total   = factcount((uint64_t)n, 2);
    seq_t strict  = strictfactcount((uint64_t)n, 2);
    seq_t g       = gcd(total, strict);
    return total / g;                     /* numerator after reduction */
}
