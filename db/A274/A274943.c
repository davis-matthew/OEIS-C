/*
 * A274943  Smallest self-descriptive number in base b, or -1 if no such
 *          number exists.
 * https://oeis.org/A274943
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, base, more
 * Formula:   smallest self‑descriptive number in base b (index n), or –1 if
 *            none exists
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* bases 2,3 have no self‑descriptive number           */
    if (n < 4)
        return -1;

    /* base 4: 1210_4 = 100 */
    if (n == 4) {
        seq_t val = 0;
        seq_t base = 4;
        val = val * base + 1;
        val = val * base + 2;
        val = val * base + 1;
        val = val * base + 0;
        return val;
    }

    /* base 5: 21200_5 = 1425 */
    if (n == 5) {
        seq_t val = 0;
        seq_t base = 5;
        val = val * base + 2;
        val = val * base + 1;
        val = val * base + 2;
        val = val * base + 0;
        val = val * base + 0;
        return val;
    }

    /* base 6 has no self‑descriptive number */
    if (n == 6)
        return -1;

    /* bases b >= 7:  (b-4), 2, 1, 0 … 0, 1, 0 … 0 */
    seq_t base = (seq_t)n;
    seq_t val  = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t d = 0;
        if (i == 0)
            d = n - 4;
        else if (i == 1)
            d = 2;
        else if (i == 2)
            d = 1;
        else if (i == n - 4)
            d = 1;
        /* all other digits are 0 */
        val = val * base + d;
    }
    return val;
}
