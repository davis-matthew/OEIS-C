/*
 * A344389  a(n) is the number of nonnegative numbers < 10^n with all digits
 *          distinct.
 * https://oeis.org/A344389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1; a(1) = 10; for 2 ≤ n ≤ 10: a(n) = a(n‑1) +
 *            9·Π_{i=1}^{n‑1}(10–i); for n > 10: a(n) = a(10) (pigeonhole
 *            principle)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* static cache for the first 11 terms (0…10) */
    static seq_t cache[11];
    static unsigned char init = 0;

    if (!init) {
        /* a(0) */
        cache[0] = 1;
        /* a(1) */
        cache[1] = 10;
        /* compute a(2)..a(10) */
        for (int k = 2; k <= 10; ++k) {
            seq_t val = 9;                   /* first digit (1–9) */
            for (int pos = 1; pos < k; ++pos) {
                val *= (seq_t)(10 - pos);    /* remaining digits */
            }
            cache[k] = cache[k - 1] + val;    /* recurrence */
        }
        init = 1;
    }

    /* return the appropriate value */
    if (n <= 10) {
        return cache[(int)n];   /* safe cast for array index */
    } else {
        return cache[10];
    }
}
