/*
 * A302258  Decimal expansion of the second radiation constant c_2 in
 *          meter-kelvin.
 * https://oeis.org/A302258
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A302258:  digits of the decimal expansion of
   the second radiation constant c₂ =
   272115870842319 / 18913000000000000.
   a(n) gives the digit at decimal position L+n+1 after the point,
   where L is the index of the first non‑zero digit.
   The function performs a long division using 64‑bit integers and
   works for any n ≥ –1. */
#include <stdbool.h>

static const unsigned long long NUM = 272115870842319ULL;
static const unsigned long long DEN = 18913000000000000ULL;

seq_t a(idx_t n)
{
    /* undefined for indices below –1 */
    if (n < -1)
        return (seq_t)0;

    /* Find the first non‑zero decimal digit once */
    static bool init = false;
    static idx_t first_non_zero;
    if (!init) {
        unsigned long long rem = NUM % DEN;
        idx_t pos = 0;
        unsigned long long digit;
        while (1) {
            rem *= 10;
            digit = rem / DEN;
            rem %= DEN;
            if (digit != 0) {
                first_non_zero = pos;
                break;
            }
            pos++;
        }
        init = true;
    }

    /* Position of the desired decimal digit */
    idx_t target = first_non_zero + n + 1;   /* zero‑based index */

    /* Compute the digit at position 'target' via long division */
    unsigned long long rem = NUM % DEN;
    unsigned long long digit = 0;
    for (idx_t i = 0; i <= target; ++i) {
        rem *= 10;
        digit = rem / DEN;
        rem %= DEN;
    }

    return (seq_t)digit;
}
