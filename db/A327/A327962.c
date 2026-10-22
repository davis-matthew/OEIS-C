/*
 * A327962  a(1) = 27846; thereafter a(n+1) = a(n) # n, where # is an
 *          operation that cycles through division, addition, subtraction and
 *          multiplication.
 * https://oeis.org/A327962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(1) = 27846; for n>1, a(n) = a(n-1) # (n-1) where # cycles
 *            through division, addition, subtraction, multiplication in this
 *            order (/,+,-,*)
 * Range:     u8 none, u16 n <= 4, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1, n==0 is undefined */
    seq_t val = 27846;             /* a(1) */
    if (n == 1) return val;

    for (idx_t k = 1; k < n; ++k) {
        switch (k % 4) {
            case 1:   /* division */
                if (val % (seq_t)k != 0) return 0;  /* sequence terminates if not integer */
                val /= (seq_t)k;
                break;
            case 2:   /* addition */
                val += (seq_t)k;
                break;
            case 3:   /* subtraction */
                val -= (seq_t)k;
                break;
            case 0:   /* multiplication */
                val *= (seq_t)k;
                break;
        }
    }
    return val;
}
