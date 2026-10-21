/*
 * A262114  Irregular triangle read by rows: row b (b >= 2) gives periodic
 *          part of digits of the base-b expansion of 1/5.
 * https://oeis.org/A262114
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) = digit_{n-2} of the concatenated repeating parts of the
 *            base‑b expansion of 1/5 for b = 2,3,4,... The period length for
 *            base b is: 4 if b mod 5 = 2 or 3 2 if b mod 5 = 4 1 if b mod 5 =
 *            0 or 1
 * Range:     u8 n <= 771, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For indices below the offset (2) the behaviour is unspecified. */
    if (n < 2)
        return (seq_t)0;

    /* The target position in the flattened sequence (0‑based). */
    unsigned long long target = n - 2;

    unsigned long long cumulative = 0;     /* number of digits generated so far */
    uint64_t base = 2;                    /* current base starting from 2 */

    for (;;) {
        /* Generate the repeating cycle of 1/5 in this base. */
        unsigned long long rem = 1;        /* remainder starts at 1 */

        for (;;) {
            unsigned long long prod = rem * (unsigned long long)base;
            unsigned long long digit = prod / 5;
            rem          = prod % 5;

            if (cumulative == target)
                return (seq_t)digit;

            cumulative++;

            /* Terminate the cycle when the remainder is 0 (terminating
               expansion) or when it returns to 1 (cycle completes). */
            if (rem == 0 || rem == 1)
                break;
        }

        base++;     /* move to the next base */
    }
}
