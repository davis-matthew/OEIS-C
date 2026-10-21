/*
 * A244809  The hexagonal spiral of Champernowne, read along the 30-degree
 *          ray.
 * https://oeis.org/A244809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = digit of Champernowne's constant at position k = 3*n*n -
 *            7*n + 5 (1‑indexed), i.e. a(n) is the (3n²-7n+5)th Champernowne
 *            digit
 * Range:     u8 n <= 10, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the position k in 1‑based indexing */
    seq_t k = (seq_t)3 * n * n - (seq_t)7 * n + 5;
    idx_t pos = (idx_t)k;               /* position in Champernowne's string */

    /* determine the decimal length d of the number containing the digit */
    idx_t d = 1;
    idx_t pow10 = 1;          /* 10^(d-1) */
    idx_t count = 9;          /* numbers with d digits */
    idx_t digitsInBlock;

    while (1) {
        digitsInBlock = count * d;
        if (pos <= digitsInBlock)
            break;
        pos -= digitsInBlock;

        d++;
        pow10 *= 10;
        count = 9 * pow10;
    }

    /* compute the number containing the desired digit */
    idx_t firstNumber = (d == 1) ? 1 : pow10;
    idx_t offset      = (pos - 1) / d;
    idx_t digitIndex  = (pos - 1) % d;
    idx_t number      = firstNumber + offset;

    /* extract the digit at digitIndex (0 = most significant) */
    unsigned char digits[20];
    idx_t tmp = number;
    for (int i = (int)d - 1; i >= 0; --i) {
        digits[i] = tmp % 10;
        tmp /= 10;
    }

    return (seq_t)digits[digitIndex];
}
