/*
 * A329480  a(n) = (1 - A075677(n))/6 if 6|(A075677(n)-1) or a(n) =
 *          (A075677(n) + 1)/6 if 6|(A075677(n)+1).
 * https://oeis.org/A329480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is defined by a 4‑column table T T(n,1) = 1 - n T(n,2) =
 *            2*n - 1 T(n,3) = T( floor((n-1)/4)+1 , (n-1)%4 + 1 ) T(n,4) =
 *            2*n a(n) = T(n,3) This directly implements the recurrence given
 *            in the OEIS entry for A329480
 * Range:     i8 n <= 255, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9959 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t t_col(idx_t n, unsigned col)
{
    switch (col) {
        case 1:
            return (seq_t)1 - (seq_t)n;
        case 2:
            return (seq_t)2 * (seq_t)n - (seq_t)1;
        case 4:
            return (seq_t)2 * (seq_t)n;
        case 3: {
            idx_t n1 = n - (idx_t)1;
            return t_col(n1 / (idx_t)4 + (idx_t)1,
                         (unsigned)(n1 % (idx_t)4) + (unsigned)1);
        }
        default:
            return (seq_t)0;   /* should never happen */
    }
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    return t_col(n, 3);
}
