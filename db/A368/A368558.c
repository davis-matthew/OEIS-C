/*
 * A368558  Number of fractions i/n that are in the Cantor set.
 * https://oeis.org/A368558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2186, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of integers i with 0 <= i <= n for which the
 * fraction i/n can be written in base 3 using only the digits 0 and 2.
 *
 * The algorithm below checks, for each i, whether the ternary expansion
 * obtained by the standard long‑division algorithm contains a digit 1.
 * If a digit 1 occurs and the remainder afterwards is non‑zero, the
 * fraction cannot belong to the Cantor set.  If the digit 1 occurs
 * only just before the expansion terminates (remainder becomes 0),
 * the fraction is in the Cantor set via the alternative expansion
 * that uses only 0 and 2.
 * This rule covers all cases, including numbers such as 1/3.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static idx_t
gcd_ui(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t i = 0; i <= n; ++i) {
        /* The endpoints 0 and 1 are always in the Cantor set. */
        if (i == 0 || i == n) {
            ++count;
            continue;
        }

        idx_t r = i;
        bool ok = true;

        /* The expansion cannot involve more than n remainders. */
        for (idx_t step = 0; step < n; ++step) {
            unsigned __int128 m = (unsigned __int128)r * 3;
            idx_t digit = (idx_t)(m / n);
            idx_t rem   = (idx_t)(m % n);

            if (digit == 1) {
                /* If the remainder becomes zero immediately after a 1,
                 * the fraction has an alternative 0‑2 expansion. */
                if (rem != 0) {
                    ok = false;
                    break;
                }
                /* remainder is zero -> terminate the loop gracefully. */
                r = 0;
                break;
            }

            r = rem;
            if (r == 0) {
                break;  /* Finite expansion ending with 0s – acceptable. */
            }
        }

        if (ok) {
            ++count;
        }
    }

    return count;
}
