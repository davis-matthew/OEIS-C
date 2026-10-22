/*
 * A362687  Binomial(n+p, n) mod n where p=7.
 * https://oeis.org/A362687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+7,7) mod n (offset = 1)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 115, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return a(n) for n >= 1  */
seq_t a(idx_t n)
{
    /* For safety, handle the degenerate case n == 0 (though never called). */
    if (n == 0)
        return 0;

    /* Compute binomial(n+7,7) multiplicatively.  The intermediate
       values fit in seq_t because the binomial coefficients grow
       moderately fast (7! = 5040).  Division at each step is exact. */
    seq_t result = 1;
    for (idx_t i = 1; i <= 7; ++i) {
        /* (n+7 - (i-1)) == n + (8 - i) */
        result = (result * ((seq_t)n + (7 - i + 1))) / (seq_t)i;
    }

    /* Take the remainder modulo n.  Cast n to seq_t so the % operator
       sees operands of the same type. */
    return result % (seq_t)n;
}
