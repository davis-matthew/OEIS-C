/*
 * A261040  Values of c such that the Diophantine equation 5*a + 3*b = c has
 *          no solutions in positive integers.
 * https://oeis.org/A261040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) is the nth positive integer c such that the Diophantine
 *            equation 5*a + 3*b = c has **no** solutions in positive integers
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm enumerates all candidates up to the bound 15 (ab = 15) and
   returns the n‑th one that has no such representation. */

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t c = 1; c <= 15; ++c) {
        int has = 0;
        for (idx_t a = 1; a <= c / 5; ++a) {
            idx_t rest = c - 5 * a;
            if (rest > 0 && rest % 3 == 0) {
                has = 1;
                break;
            }
        }
        if (!has) {
            ++count;
            if (count == n) {
                return (seq_t)c;
            }
        }
    }
    return 0;
}
