/*
 * A291161  Sequence of black numbers on the double zero (American) roulette
 *          wheel read clockwise.
 * https://oeis.org/A291161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A291161 – Sequence of black numbers on the double‑zero roulette wheel
    read clockwise.   The sequence is periodic of period 18.  It is given
    piecewise as

        r = ((n-1) mod 18) + 1          (1 ≤ r ≤ 18)

        a(n) = 28             when r = 1
              = 26             when r = 2
              = 11             when r = 3
              = 19 + (-1)^r * (r-3)   for 4 ≤ r ≤ 8
              = 13             when r = 9
              = 19 - (-1)^r * (r-1)   for 10 ≤ r ≤ 18
*/
seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined for n<1 – return 0 */
        return 0;

    /* r is the position within one period (1…18) */
    idx_t r = (n - 1) % 18 + 1;
    int rI = (int)r;               /* small, fits in signed int safely */
    int value;

    switch (rI) {
    case 1: value = 28; break;
    case 2: value = 26; break;
    case 3: value = 11; break;
    case 9: value = 13; break;

    /* 4 ≤ r ≤ 8  : a(n) = 19 + (-1)^r * (r-3) */
    case 4: case 5: case 6: case 7: case 8:
        {
            int sign = (rI % 2 == 0) ? 1 : -1;
            value = 19 + sign * (rI - 3);
        }
        break;

    /* 10 ≤ r ≤ 18 : a(n) = 19 - (-1)^r * (r-1) */
    default:
        {
            int sign = (rI % 2 == 0) ? 1 : -1;
            value = 19 - sign * (rI - 1);
        }
        break;
    }

    return (seq_t)value;
}
