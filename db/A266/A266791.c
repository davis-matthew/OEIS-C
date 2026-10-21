/*
 * A266791  Decimal representation of the middle column of the "Rule 61"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-(-1)^n + 5627*2^(n-9) - 3)/6 for n>9
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static const seq_t first[10] = {
    1, 3, 7, 14, 29, 58, 117, 234, 468, 937
};

seq_t a(idx_t n)
{
    if (n < 10) return first[n];

    idx_t shift = n - 9;
    int sign = (n & 1) ? 1 : -1;          /* +1 for odd n, -1 for even n */

    /* If seq_t is small enough, use 128‑bit maths to avoid overflow */
    if (sizeof(seq_t) <= 8 && shift < 127) {
        __int128 num = (__int128)5627 * ((__int128)1 << shift);
        num -= 3;          /* subtract 3 */
        num += sign;       /* add +1 or -1 */
        num /= 6;          /* integer division */
        return (seq_t)num;
    }

    /* For larger seq_t (arbitrary‑precision) use the generic arithmetic */
    seq_t term = pow2(shift);               /* 2^(n-9) */
    term = term * (seq_t)5627;              /* 5627*2^(n-9) */
    term -= (seq_t)3;                       /* subtract 3 */
    if (sign > 0) term += (seq_t)1;         /* add 1 for odd n */
    else           term -= (seq_t)1;        /* subtract 1 for even n */
    term /= (seq_t)6;                       /* divide by 6 */

    return term;
}
