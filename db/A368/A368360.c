/*
 * A368360  Comma transform of the even numbers.
 * https://oeis.org/A368360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 2; for n >= 1, a(n) = 10 * ((2*n) mod 10) +
 *            most_significant_digit(2*n + 2)
 * Range:     u8 n <= 499, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: most significant decimal digit of a seq_t value */
static seq_t first_digit(seq_t value)
{
    while (value >= (seq_t)10) {
        value = value / (seq_t)10;
    }
    return value;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)2;
    }

    seq_t prev = ((seq_t)2 * (seq_t)n);   /* 2*n = last even number before n */
    seq_t last_digit = prev % (seq_t)10;  /* last decimal digit of 2*n */
    seq_t curr = prev + (seq_t)2;         /* 2*n + 2 = next even number */
    seq_t first_digit_of_curr = first_digit(curr);

    return last_digit * (seq_t)10 + first_digit_of_curr;
}
