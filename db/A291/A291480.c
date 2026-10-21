/*
 * A291480  The arithmetic function u(n,7,8).
 * https://oeis.org/A291480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = u(n,7,8) where u(n,a,b) = smallest divisor of n that lies
 *            in the interval [a,b]; if n has no such divisor, u(n,a,b) = b.
 *            For n < 7 the sequence is defined explicitly: a(1)=a(2)=a(3)=49,
 *            a(4)=36, a(5)=45, a(6)=49
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first six terms exactly as defined in the sequence */
    if (n <= 6) {
        switch ((int)n) {
        case 1:
        case 2:
        case 3:
            return (seq_t)49;
        case 4:
            return (seq_t)36;
        case 5:
            return (seq_t)45;
        case 6:
            return (seq_t)49;
        default:
            /* this case will never happen because n <= 6 */
            return (seq_t)0;
        }
    }

    /* for n >= 7, find the smallest divisor of n that lies between
       7 and 49 inclusive.  If none is found, return 49. */
    for (idx_t d = 7; d <= 49 && d <= n; ++d) {
        if (n % d == 0)
            return (seq_t)d;
    }
    return (seq_t)49;
}
