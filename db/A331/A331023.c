/*
 * A331023  Numerator of number of factorizations of n divided by number of
 *          strict factorizations of n A001055(n)/A045778(n).
 * https://oeis.org/A331023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Numerator of A001055(n)/A045778(n)
 * Range:     u8 n <= 2879, u16 n <= 7409, u32 n <= 7409, u64 n <= 7409,
 *            u128 n <= 7409, big any n
 * Helpers:   factcount, gcd, strictfactcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factcount.h"
#include "../utils/strictfactcount.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1 (offset 1). */
    if (n < 1) return 0;                  /* undefined for n = 0, but this case will not occur */

    seq_t total   = factcount((uint64_t)n, 2);
    seq_t strict  = strictfactcount((uint64_t)n, 2);
    seq_t g       = gcd(total, strict);
    return total / g;                     /* numerator after reduction */
}
