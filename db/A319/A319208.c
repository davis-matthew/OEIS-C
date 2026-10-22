/*
 * A319208  a(n) = 1*2*3*4*5*6*7 + 8*9*10*11*12*13*14 + 15*16*17*18*19*20*21 +
 *          ... + (up to n).
 * https://oeis.org/A319208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/7)} (7*i)!/(7*i-7)! + Sum_{j=1..6}
 *            (1-sign((n-j) mod 7)) * (Product_{i=1..j} n-i+1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 27, u64 n <= 426,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. For n==0 we simply return 0. */
    if (n == 0) return (seq_t)0;

    seq_t sum = (seq_t)0;

    /* Number of complete 7‑term blocks before n. */
    idx_t full_blocks = n / 7;

    /* Sum over all complete blocks. */
    for (idx_t i = 0; i < full_blocks; ++i) {
        seq_t prod = (seq_t)1;
        /* The block starts at 7*i + 1 and ends at 7*i + 7. */
        idx_t start = 7 * i + 1;
        for (idx_t k = 0; k < 7; ++k) {
            prod *= (seq_t)(start + k);
        }
        sum += prod;
    }

    /* Add the partial product of the remaining numbers of the last block. */
    idx_t rem = n % 7;
    if (rem > 0) {
        seq_t prod = (seq_t)1;
        /* Product of the numbers n, n-1, …, n-rem+1. */
        for (idx_t k = 0; k < rem; ++k) {
            prod *= (seq_t)(n - k);
        }
        sum += prod;
    }

    return sum;
}
