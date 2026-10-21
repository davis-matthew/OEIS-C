/*
 * A256290  Numbers which have only digits 4 and 5 in base 10.
 * https://oeis.org/A256290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) builds the n-th number containing only digits 4 and 5 by
 *            interpreting the offset within the chosen digit length as binary
 *            and mapping 0→4, 1→5
 * Range:     u8 n <= 6, u16 n <= 62, u32 n <= 1022, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the length of the number */
    idx_t cumulative = 0;
    idx_t pow = 1;        /* will become 2^len after the shift */
    idx_t len = 0;

    while (1) {
        pow <<= 1;          /* pow = 2^len now */
        cumulative += pow; /* cumulative = sum_{i=1}^{len} 2^i */
        len++;
        if (cumulative >= n)
            break;
    }

    /* offset of the desired number among all len‑digit numbers */
    idx_t prev_cumulative = cumulative - pow;
    idx_t offset = n - prev_cumulative - 1;

    /* construct the number by mapping binary bits to digits 4 and 5 */
    seq_t result = 0;
    for (idx_t i = len; i > 0; --i) {
        idx_t shift = i - 1;
        int bit = (offset >> shift) & 1;
        seq_t digit = bit ? 5 : 4;
        result = result * 10 + digit;
    }

    return result;
}
