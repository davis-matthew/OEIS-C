/*
 * A244807  The hexagonal spiral of Champernowne, read along the East (or
 *          90-degree) ray.
 * https://oeis.org/A244807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digit of the Champernowne constant at position
 *            3*n^2-8*n+6 (the East ray of the hexagonal Champernowne spiral)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t champernowne_digit(idx_t pos)
{
    /* find the length of the numbers that contain position 'pos' */
    idx_t digit_len = 1;
    idx_t power10 = 1;          /* 10^(digit_len-1) */
    idx_t count = 9;            /* numbers of current digit_len */
    idx_t block_digits = count * digit_len;
    idx_t cumulative = block_digits;

    while (cumulative < pos) {
        digit_len++;
        power10 *= 10;
        count = 9 * power10;
        block_digits = count * digit_len;
        cumulative += block_digits;
    }

    idx_t prev_cumulative = cumulative - block_digits;
    idx_t offset = pos - prev_cumulative - 1;   /* zero‑based offset inside this block */
    idx_t number_index = offset / digit_len;    /* zero‑based index of the number within the block */
    idx_t digit_index = offset % digit_len;     /* position of the desired digit inside the number */

    /* compute the digit without constructing the whole number.
       divisor = 10^(digit_len-1-digit_index) */
    idx_t divisor = 1;
    for (idx_t e = 0; e < digit_len - 1 - digit_index; ++e)
        divisor *= 10;

    /* leading part = 10^digit_index (the contribution of the leading digit of 10^(digit_len-1)) */
    idx_t leading = 1;
    for (idx_t e = 0; e < digit_index; ++e)
        leading *= 10;

    idx_t val = (leading + number_index / divisor) % 10;
    return (seq_t)val;
}

seq_t a(idx_t n)
{
    idx_t N = n;
    idx_t pos = 3 * N * N - 8 * N + 6;   /* position in Champernowne */
    return champernowne_digit(pos);
}
