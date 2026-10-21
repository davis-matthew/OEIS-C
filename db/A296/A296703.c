/*
 * A296703  Numbers whose base-7 digits d(m), d(m-1), ... d(0) have #(rises) =
 *          #(falls); see Comments.
 * https://oeis.org/A296703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n-th positive integer whose base‑7 digits have equal
 *            number of rises and falls (a rise is a digit increase, a fall is
 *            a digit decrease)
 * Range:     u8 n <= 138, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t x = 0;

    for (;;) {
        ++x;                   /* start from 1 */

        /* Count rises and falls in base‑7 representation. */
        int rises = 0, falls = 0;
        seq_t tmp = x;
        seq_t prev = tmp % (seq_t)7;
        tmp /= (seq_t)7;

        while (tmp > 0) {
            seq_t cur = tmp % (seq_t)7;
            if (cur < prev)
                falls += 1;
            else if (cur > prev)
                rises += 1;
            prev = cur;
            tmp /= (seq_t)7;
        }

        if (rises == falls) {
            ++count;
            if (count == n)
                return x;
        }
    }
}
