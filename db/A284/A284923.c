/*
 * A284923  Numbers with digits 2 and 9 only.
 * https://oeis.org/A284923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth smallest integer containing only decimal digits 2 and 9. */
/* The terms are sorted in increasing numeric order. */

seq_t a(idx_t n) {
    /* Find the smallest number of digits L such that
       the total count of all numbers with ≤ L digits is ≥ n. */
    idx_t len = 1;
    idx_t cumulative = 0;      /* total numbers of lengths < len */
    idx_t count = 2;           /* numbers of current length = 2^len */

    while (n > cumulative + count) {
        cumulative += count;
        count <<= 1;          /* numbers double when length increases by 1 */
        len++;
    }

    /* Zero‑based index within the block of length `len`. */
    idx_t offset = n - cumulative - 1;  /* 0 ≤ offset < count */

    seq_t result = 0;
    /* Construct the decimal number digit by digit, from most to least
       significant.  The most significant bit of `offset` corresponds
       to the leading digit. */
    for (idx_t i = 0; i < len; i++) {
        idx_t bit_index = len - 1 - i;
        idx_t bit = (offset >> bit_index) & 1;
        seq_t digit = bit ? 9 : 2;
        result = result * 10 + digit;
    }
    return result;
}
