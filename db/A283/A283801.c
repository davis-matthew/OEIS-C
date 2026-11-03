/*
 * A283801  Concatenation of the first n odd composite numbers (A071904).
 * https://oeis.org/A283801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = concatenation of the first n odd composite numbers
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-01)
 * Helpers:   is_prime, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    seq_t result = 0;
    idx_t count   = 0;
    idx_t num     = 9;              /* first odd composite */

    while (count < n)
    {
        if (!is_prime(num))        /* composite odd number */
        {
            seq_t part = (seq_t)num;
            int     d = num_digits(part);
            seq_t mult = pow10((idx_t)d);
            result = result * mult + part;
            count++;
        }
        num += 2;                  /* skip even numbers */
    }
    return result;
}
