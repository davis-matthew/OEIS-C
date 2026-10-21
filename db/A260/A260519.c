/*
 * A260519  Concatenate the positions of digits 0, 1,..., 9 in the decimal
 *          representation of n, using 1 for the rightmost digit etc., and 0
 *          when the digit does not occur.
 * https://oeis.org/A260519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the positions (1‑indexed from the right)
 *            of each digit 0–9 in the decimal representation of n. For a
 *            digit that does not occur, a single 0 is concatenated
 * Range:     u8 none, u16 none, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* positions that each digit occupies (1‑indexed from right) */
    int pos_counts[10] = {0};
    int pos_lists[10][20] = {0};      /* 20 is enough for 64‑bit numbers */

    /* Special case n == 0: its only digit is 0 at position 1 */
    if (n == 0) {
        pos_counts[0] = 1;
        pos_lists[0][0] = 1;
    } else {
        idx_t tmp = n;
        int pos = 1;                  /* rightmost digit has position 1 */
        while (tmp > 0) {
            int digit = tmp % 10;
            pos_lists[digit][pos_counts[digit]++] = pos;
            tmp /= 10;
            pos++;
        }
    }

    /* Pre‑compute powers of ten up to 20 digits. */
    seq_t pow10[21];
    pow10[0] = 1;
    for (int i = 1; i <= 20; ++i)
        pow10[i] = pow10[i - 1] * 10;

    seq_t result = 0;
    for (int d = 0; d <= 9; ++d) {
        if (pos_counts[d] == 0) {
            /* digit d does not occur, append a single 0 */
            result = result * 10 + 0;
        } else {
            for (int i = 0; i < pos_counts[d]; ++i) {
                int p = pos_lists[d][i];
                /* number of decimal digits of position p */
                int p_len = 0;
                int tt = p;
                do {
                    tt /= 10;
                    ++p_len;
                } while (tt > 0);
                result = result * pow10[p_len] + (seq_t)p;
            }
        }
    }
    return result;
}
