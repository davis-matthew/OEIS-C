/*
 * A244813  The hexagonal spiral of Champernowne, read along the West (or
 *          270-degree) ray.
 * https://oeis.org/A244813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = digit of Champernowne’s constant at position 3*n^2 - 5*n
 *            + 3
 * Range:     u8 n <= 10, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const idx_t pow10[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL,
    10000000000000000000ULL
};

seq_t a(idx_t n)
{
    /* k = 3*n^2 - 5*n + 3 (index into Champernowne’s concatenation) */
    seq_t k = (seq_t)n * ((seq_t)n * 3 - 5) + 3;
    idx_t pos = (idx_t)k;          /* 1‑based position in the string */

    /* Find the digit length d of the number that contains this position */
    idx_t d = 1;
    idx_t digits_of_d = 9ULL;      /* 9 * 10^(d-1) * d */
    while (pos > digits_of_d) {
        pos -= digits_of_d;        /* skip whole block of d‑digit numbers */
        d++;
        digits_of_d = 9ULL * pow10[d - 1] * d;
    }

    /* Now pos is within the block of d‑digit numbers */
    idx_t offset = pos - 1;           /* zero‑based offset in this block */
    idx_t num_index = offset / d;     /* which number of this block */
    idx_t digit_index = offset % d;   /* which digit inside that number */

    idx_t number = pow10[d - 1] + num_index;
    idx_t divisor = pow10[d - 1 - digit_index];
    seq_t digit = (seq_t)((number / divisor) % 10);

    return digit;
}
