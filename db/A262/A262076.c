/*
 * A262076  The first of seven consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of thirteen
 *          consecutive positive integers.
 * https://oeis.org/A262076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+3148*a(n-2)-3148*a(n-3)-a(n-4)+a(n-5); Initial
 *            values: a(1)=26, a(2)=598, a(3)=90688, a(4)=1891916,
 *            a(5)=285495236 The implementation computes the recurrence
 *            iteratively, using only arithmetic on seq_t and idx_t. No
 *            dynamic allocation or memoisation is required; the computation
 *            is O...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 as a safe default. */
    if (n == 0)
        return 0;

    /* Handle the first five terms directly. */
    switch ((int)n) {
        case 1: return (seq_t)26;
        case 2: return (seq_t)598;
        case 3: return (seq_t)90688;
        case 4: return (seq_t)1891916;
        case 5: return (seq_t)285495236;
    }

    /* Use the last five computed values; initialise with a(5)…a(1). */
    seq_t x1 = (seq_t)285495236; /* a(n-1)  */
    seq_t x2 = (seq_t)1891916;   /* a(n-2)  */
    seq_t x3 = (seq_t)90688;     /* a(n-3)  */
    seq_t x4 = (seq_t)598;       /* a(n-4)  */
    seq_t x5 = (seq_t)26;        /* a(n-5)  */

    /* Iteratively compute a(6) … a(n). */
    for (idx_t i = 6; i <= n; ++i) {
        /* Rearrange the recurrence to avoid unsigned underflow: */
        /* a = (x1 - x4 + x5) + 3148 * (x2 - x3) */
        seq_t part1 = x1 - x4 + x5;          /* x1 - x4 + x5 */
        seq_t part2 = x2 - x3;              /* x2 - x3  (non‑negative) */
        seq_t new_val = part1 + (seq_t)3148 * part2;

        /* Shift the window: new term becomes the newest “previous”. */
        x5 = x4;
        x4 = x3;
        x3 = x2;
        x2 = x1;
        x1 = new_val;
    }

    return x1; /* x1 now holds a(n) */
}
