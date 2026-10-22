/*
 * A380109  Decimal expansion of 223/71.
 * https://oeis.org/A380109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(1)=3; for n≥2 a(n) is the n‑th decimal digit of 223/71
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t digits[70];
    static idx_t period = 0;
    static int initialized = 0;
    if (!initialized) {
        idx_t rem = 10;   /* remainder after the integer part 3 */
        idx_t i = 0;
        do {
            rem *= 10;
            digits[i++] = (seq_t)(rem / 71);
            rem %= 71;
        } while (rem != 10);
        period = i;
        initialized = 1;
    }
    if (n == 1)
        return (seq_t)3;
    idx_t pos = (n - 2) % period;
    return digits[pos];
}
