/*
 * A341770  Largest number m on the square spiral board such that it takes n
 *          steps to reach square 1 from square m along the shortest path
 *          without stepping on any prime number.
 * https://oeis.org/A341770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1,8,23,34,61,62,97 for n=0..6; a(n) = 4*n^2 - 9*n + 5 for
 *            n >= 7
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t base[7] = {1, 8, 23, 34, 61, 62, 97};
    if (n < 7)
        return base[n];
    unsigned long long nn = (unsigned long long)n;
    unsigned long long val = 4ULL * nn * nn - 9ULL * nn + 5ULL;
    return (seq_t)val;
}
