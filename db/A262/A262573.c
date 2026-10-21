/*
 * A262573  Concatenation of the numbers from 1 to n but omitting 3.
 * https://oeis.org/A262573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the first n positive integers, with the
 *            integer 3 omitted from the list
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline int num_digits(idx_t x)
{
    int d = 0;
    do { d++; x /= 10; } while (x > 0);
    return d;
}

static inline seq_t pow10_int(int e)
{
    seq_t r = 1;
    while (e--) r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* values below the offset are irrelevant */

    seq_t result = 0;
    for (idx_t pos = 1; pos <= n; ++pos) {
        idx_t num = (pos < 3) ? pos : pos + 1; /* skip the integer 3 */
        int d   = num_digits(num);
        seq_t mult = pow10_int(d);
        result = result * mult + (seq_t)num;
    }
    return result;
}
