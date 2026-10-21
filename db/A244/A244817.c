/*
 * A244817  The hexagonal spiral of Champernowne, read along the 150-degree
 *          ray.
 * https://oeis.org/A244817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the digit at position k = 3*n*n - 3*n + 1 of
 *            Champernowne’s * concatenation of the positive integers (1, 2,
 *            3, …). * The sequence A244817 lists these digits with a(1) = 1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t champernowne_digit(idx_t pos)
{
    /* Find the digit at 1‑based position pos in "123456789101112…". */
    idx_t k = pos;          /* remaining offset within the current block   */
    idx_t d = 1;            /* current number of digits in the block       */
    idx_t count = 9;        /* numbers with d digits (9 * 10^(d-1))        */
    idx_t block_digits = count * d;  /* digits contributed by this block */

    /* Locate the block containing the desired position. */
    while (k > block_digits) {
        k -= block_digits;
        ++d;
        count *= 10;                     /* 9 * 10^(d-1)                      */
        block_digits = count * d;        /* digits in the new block            */
    }

    /* Starting number of the block (10^(d-1)). */
    idx_t start = 1;
    for (idx_t i = 1; i < d; ++i)
        start *= 10;

    /* Zero‑based index of the number within this block. */
    idx_t offset = (k - 1) / d;
    idx_t number = start + offset;      /* actual number containing the digit */

    /* Zero‑based position of the digit within that number. */
    idx_t digit_pos = (k - 1) % d;       /* 0 for leftmost digit */

    /* Compute 10^(d-1-digital_pos) for the divisor. */
    idx_t pow10 = 1;
    for (idx_t i = 0; i < d - 1 - digit_pos; ++i)
        pow10 *= 10;

    /* Extract the required digit. */
    return (seq_t)((number / pow10) % 10);
}

extern seq_t a(idx_t n)
{
    /* k = 3*n^2 - 3*n + 1 (1‑based) */
    idx_t k = n * (3 * n - 3) + 1;
    return champernowne_digit(k);
}
