/*
 * A276194  Odd numbers whose binary representation contains an even number of
 *          1's and at least one 0.
 * https://oeis.org/A276194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate odd numbers, skip numbers that are all 1's in binary, and
   count those whose popcount is even (thus an even number of 1's).  The first
   valid odd number is 5 (binary 101), followed by 9, 17, 23, ... */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0, return 0 */
    idx_t found = 0;               /* how many valid numbers we have seen */
    for (idx_t x = 1; ; x += 2) {   /* iterate over odd numbers only */
        /* skip numbers that are all 1's in binary (e.g., 1, 3, 7, 15, ...) */
        if (((x + 1) & x) == 0) continue;

        /* popcount even: count the number of 1-bits */
        idx_t temp = x;
        idx_t pop = 0;
        while (temp) {
            pop += temp & 1U;
            temp >>= 1;
        }
        if ((pop & 1U) == 0) {                /* even popcount */
            ++found;
            if (found == n) {
                return (seq_t)x;               /* return as seq_t type */
            }
        }
    }
}
