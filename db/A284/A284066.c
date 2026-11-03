/*
 * A284066  Numbers whose smallest decimal digit is 5.
 * https://oeis.org/A284066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Find the n-th positive integer whose decimal
   representation contains only digits 5–9 and at least one digit 5.
   The sequence is A284066. */
#include <stdbool.h>

/* Helper: returns true if x has all decimal digits ≥5 and at least one digit 5 */
static bool contains_min_five(idx_t x)
{
    bool has5 = false;
    while (x) {
        idx_t d = x % 10;
        if (d < 5) return false;   /* digit less than 5 ⇒ cannot be in sequence */
        if (d == 5) has5 = true;    /* remember that we have a 5 digit */
        x /= 10;
    }
    return has5;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Offset starts at 1; behavior for n<1 is undefined */

    idx_t count = 0;   /* number of terms found so far */
    idx_t num    = 0;   /* current candidate integer */

    while (1) {
        if (contains_min_five(num)) {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
        ++num;
    }
}
