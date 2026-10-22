/*
 * A342484  Decimal expansion of number of newtons (N) in 1 pound-force (lbf).
 * https://oeis.org/A342484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = decimal digit #n of the exact value 1 lbf =
 *            4.4482216152605 N
 * Range:     u8 none, u16 none, u32 none, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* exact rational representation of 4.4482216152605 = 44482216152605 / 10000000000000 */
    const seq_t NUM = (seq_t)44482216152605ULL;
    const seq_t DEN = (seq_t)10000000000000ULL;

    static seq_t digits[13];          /* fractional digits 1..13 */
    static seq_t integer_part;        /* integer part (4) */
    static bool initialized = false;  /* one‑time initialization */

    if (n == 0)           /* offset starts at 1; 0 is undefined, return 0 safety */
        return 0;

    if (!initialized)
    {
        integer_part = NUM / DEN;     /* 4 */
        seq_t rem = NUM % DEN;
        for (idx_t i = 0; i < 13; ++i)
        {
            rem = rem * 10;
            digits[i] = rem / DEN;    /* ith fractional digit */
            rem = rem % DEN;
        }
        initialized = true;
    }

    if (n == 1)                          /* first term is the integer part */
        return integer_part;

    idx_t pos = n - 2;                    /* position in digits[] (0‑based) */
    if (pos < 13)                         /* n ≤ 14 */
        return digits[pos];
    return 0;                             /* digits after the terminal decimal are 0 */
}
