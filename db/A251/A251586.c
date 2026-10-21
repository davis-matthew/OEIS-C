/*
 * A251586  a(n) = 6^(n-4) * (n+1)^(n-6) * (125*n^4 + 810*n^3 + 2095*n^2 +
 *          2586*n + 1296).
 * https://oeis.org/A251586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   poly
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 6^(n-4) * (n+1)^(n-6) *
 *            (125*n^4 + 810*n^3 + 2095*n^2 + 2586*n + 1296)
 *
 * The implementation evaluates the expression exactly.
 * It uses exponentiation‑by‑squaring for the powers of 6
 * and n+1, handling negative exponents by dividing instead.
 */

#include <stdint.h>
#include "../utils/poly.h"

/* local power helper: base^exp for unsigned exp via binary exponentiation */
static seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t value = poly(n);

    /* Handle the 6^(n-4) factor */
    if (n >= 4)
        value *= pow_seq_local((seq_t)6, n - 4);
    else
        value /= pow_seq_local((seq_t)6, 4 - n);

    /* Handle the (n+1)^(n-6) factor */
    if (n >= 6)
        value *= pow_seq_local((seq_t)(n + 1), n - 6);
    else
        value /= pow_seq_local((seq_t)(n + 1), 6 - n);

    return value;
}
