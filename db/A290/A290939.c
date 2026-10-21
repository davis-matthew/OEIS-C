/*
 * A290939  Number of 5-cycles in the n-triangular graph.
 * https://oeis.org/A290939
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12/5 * binomial(n,4) * (n^2 + 7*n - 34)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // For n < 4 the binomial coefficient is zero, matching OEIS offset.
    if (n < 4) {
        return 0;
    }

    seq_t nn = (seq_t)n;

    // Compute binomial(n,4) = n*(n-1)*(n-2)*(n-3)/24
    seq_t binom = nn * (nn - 1) * (nn - 2) * (nn - 3);
    binom /= 24;              // division after multiplication, safe in 128-bit

    // Polynomial part: n^2 + 7*n - 34
    seq_t poly = nn * nn + 7 * nn - 34;

    // Use 128-bit intermediate to avoid overflow for 64-bit seq_t
    unsigned __int128 prod = (unsigned __int128)binom * (unsigned __int128)poly;
    prod = prod * 12;         // multiply by 12
    prod = prod / 5;          // divide by 5 (exact division assured by formula)

    return (seq_t)prod;
}
