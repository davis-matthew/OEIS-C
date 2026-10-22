/*
 * A319872  a(n) = 8*7*6*5*4*3*2*1 + 16*15*14*12*11*10*9 + ... + (up to the
 *          n-th term).
 * https://oeis.org/A319872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..7} (floor((n-j)/8)-floor((n-j-1)/8)) *
 *            (Product_{i=1..j} n-i-j+8+1) + Sum_{j=1..n}
 *            (floor(j/8)-floor((j-1)/8)) * (Product_{i=1..8} j-i+1)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 23, u64 n <= 223,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t k = 8;
    seq_t result = 0;

    /* Contribution from multiples of k (second sum) */
    for (idx_t j = k; j <= n; j += k) {
        seq_t prod = 1;
        for (idx_t t = 0; t < k; ++t) {
            prod *= (seq_t)(j - t);
        }
        result += prod;
    }

    /* Contribution from the single j = n % k term (first sum) */
    idx_t r = n % k;
    if (r != 0) {
        idx_t start = n - r + k; /* first factor of the product */
        seq_t prod = 1;
        for (idx_t t = 0; t < r; ++t) {
            prod *= (seq_t)(start - t);
        }
        result += prod;
    }

    return result;
}
