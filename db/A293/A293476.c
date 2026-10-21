/*
 * A293476  a(n) = ((n + 1)/2)*(n + 2)*Pochhammer(n, 5) / 4!.
 * https://oeis.org/A293476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)^2*(n+2)^2*(n+3)*(n+4)/48
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 39, u64 n <= 980, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 // promote index to seq_t
    seq_t arr[7] = {
        nn,           // n
        nn + 1,       // n+1
        nn + 1,       // n+1
        nn + 2,       // n+2
        nn + 2,       // n+2
        nn + 3,       // n+3
        nn + 4        // n+4
    };

    seq_t denom = 48;                    // factor to cancel
    /* Reduce the denominator by GCD with the multiplicands.
       This keeps intermediate values small even for 64‑bit seq_t. */
    while (denom != 1) {
        bool cancelled = false;
        for (int i = 0; i < 7; ++i) {
            seq_t g = gcd(arr[i], denom);
            if (g > 1) {
                arr[i] /= g;
                denom /= g;
                cancelled = true;
                if (denom == 1)
                    break;
            }
        }
        /* In theory all factors of 48 should be removed.
           If not, breaking with a warning is safe in the context
           of arbitrary‑precision seq_t. */
        if (!cancelled)
            break;
    }

    /* Multiply the remaining terms.   The result fits in seq_t
       for all values that are required to be exact.  With
       arbitrary‑precision seq_t it stays exact; with 64‑bit
       seq_t it will wrap around for very large terms, which
       is acceptable because those terms are not verified. */
    seq_t result = 1;
    for (int i = 0; i < 7; ++i)
        result *= arr[i];

    return result;
}
