/*
 * A371054  Numbers whose ternary representation consists of a run of 1's,
 *          then a run of 0's, then a run of 2's.
 * https://oeis.org/A371054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A371054 – Numbers whose ternary representation consists of
    a run of 1's, then a run of 0's, then a run of 2's.
    The function obtains the nth smallest such number by:
    1. Determining the total ternary length L that contains the nth
       value (counting C(L‑1,2) possibilities for each length).
    2. Selecting the run lengths k1, k0, k2 from the offset within that
       block.
    3. Reconstructing the number digit by digit. */
seq_t a(idx_t n)
{
    /* Offset is 1; invalid indices return 0. */
    if (n < 1)
        return 0;

    /* Find the ternary length L that contains the nth value. */
    idx_t remaining = n;
    idx_t L = 3;                     /* length must be at least 3 */
    idx_t count;
    while (1) {
        count = ((L - 1) * (L - 2)) / 2;   /* C(L‑1,2) possibilities */
        if (remaining <= count)
            break;
        remaining -= count;
        ++L;
    }

    /* Offset within the block for the found length. */
    idx_t pos   = remaining - 1;     /* 0‑based */
    idx_t k1 = 1;                    /* at least one 1 */
    idx_t k2;

    /* Determine run lengths by iterating over possible k1. */
    while (1) {
        idx_t choices_k2 = L - k1 - 1;      /* k2 ranges 1 .. L‑k1‑1 */
        if (pos < choices_k2) {
            k2 = pos + 1;                   /* k2 is 1‑based within block */
            break;
        }
        pos -= choices_k2;
        ++k1;
    }

    /* Remaining zeros. */
    idx_t k0 = L - k1 - k2;

    /* Construct the number in base 3. */
    seq_t result = 0;

    /* Run of 1's */
    for (idx_t i = 0; i < k1; ++i)
        result = result * 3 + 1;

    /* Run of 0's */
    for (idx_t i = 0; i < k0; ++i)
        result *= 3;                /* adding 0 */

    /* Run of 2's */
    for (idx_t i = 0; i < k2; ++i)
        result = result * 3 + 2;

    return result;
}
