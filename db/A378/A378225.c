/*
 * A378225  Dirichlet inverse of A067824.
 * https://oeis.org/A378225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1; for n>1, a(n)=0 if n is not square‑free, otherwise
 *            a(n)=2·(-1)^Ω(n), where Ω(n) is the total number of prime
 *            factors of n
 * Range:     i8 n <= 9698, i16 n <= 9698, i32 n <= 9698, i64 n <= 9698,
 *            i128 n <= 9698, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t t = n;
    idx_t omega = 0;

    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            int cnt = 0;
            do {
                t /= p;
                ++cnt;
            } while (t % p == 0);

            if (cnt > 1)          /* square factor found */
                return (seq_t)0;
            omega += cnt;
        }
    }
    if (t > 1)                    /* remaining prime factor */
        omega += 1;

    seq_t sign = (omega % 2 == 0) ? (seq_t)1 : (seq_t)-1;
    return sign * (seq_t)2;
}
