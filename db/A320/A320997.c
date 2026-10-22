/*
 * A320997  An absolute lower bound on the number of components in perfect
 *          systems of difference sets (PSDS).
 * https://oeis.org/A320997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 5*floor(n/3) + 0 if n mod 3 = 0 1 (n ≤ 4) or 2 (n ≥ 7) if
 *            n mod 3 = 1 3 if n mod 3 = 2
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k   = n / 3;      /* floor(n/3) */
    idx_t rem = n % 3;      /* n mod 3 */
    seq_t res;

    if (rem == 0) {
        /* a(n) = 5*k */
        res = 5 * (seq_t)k;
    } else if (rem == 1) {
        if (k <= 1)
            /* n = 1 or 4: offset +1 */
            res = 5 * (seq_t)k + 1;
        else
            /* n ≥ 7: offset +2 */
            res = 5 * (seq_t)k + 2;
    } else { /* rem == 2 */
        /* n ≡ 2 (mod 3): offset +3 */
        res = 5 * (seq_t)k + 3;
    }

    return res;
}
