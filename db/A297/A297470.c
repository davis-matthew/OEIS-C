/*
 * A297470  Number of maximal matchings in the n-barbell graph.
 * https://oeis.org/A297470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*ceil(n/2)-1)!!)^2 if n is odd, a(n) =
 *            2*((2*ceil(n/2)-1)!!)^2 if n is even
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Double factorial of odd numbers: (2k-1)!! = 1·3·5·...·(2k-1). */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1 */
    idx_t k = (n + 1) / 2;          /* ceil(n/2) */
    seq_t prod = 1;
    for (idx_t i = 1; i <= k; ++i) {
        prod *= (seq_t)(2 * i - 1);
    }
    seq_t result = prod * prod;    /* square the odd double factorial */
    if ((n & 1) == 0) {            /* even n → multiply by 2 */
        result *= 2;
    }
    return result;
}
