/*
 * A287747  Concatenation {n, n + 1,.., n + 9}.
 * https://oeis.org/A287747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concat(n, n+1, ..., n+9)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 18, u128 n <= 998,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* index below offset, defined arbitrarily */
        return 0;

    seq_t res = (seq_t)n;       /* start with the first term */

    for (idx_t i = 1ULL; i < 10ULL; ++i) {
        idx_t next = n + i;          /* next integer to concatenate */

        /* compute number of decimal digits of 'next' */
        idx_t tmp   = next;
        idx_t digits = 0;
        while (tmp > 0) {
            tmp   /= 10;
            digits++;
        }

        /* compute 10^digits */
        seq_t pow10 = 1;
        for (idx_t k = 0; k < digits; ++k)
            pow10 *= 10;

        /* append 'next' to the current result */
        res = res * pow10 + (seq_t)next;
    }

    return res;
}
