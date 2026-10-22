/*
 * A381705  Length of iteration sequence of shortest unimodal Collatz (3x+1)/2
 *          sequence that begins with exactly n increases and ends with
 *          continuous decreases until reaching 1.
 * https://oeis.org/A381705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 3^(n-1) + 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to a seq_t for arithmetic */
    seq_t n_val = (seq_t)n;

    /* Fast exponentiation of 3^(n-1) */
    seq_t pow = 1;          /* accumulator for 3^(n-1) */
    seq_t base = (seq_t)3;  /* current power of 3 being squared */
    idx_t e = n - 1;        /* exponent (non‑negative because n ≥ 1) */

    while (e > 0) {
        if (e & 1) {
            pow *= base;
        }
        base *= base;
        e >>= 1;
    }

    /* Add n, the computed power, and the final +1 */
    n_val += pow;
    n_val += 1;

    return n_val;
}
