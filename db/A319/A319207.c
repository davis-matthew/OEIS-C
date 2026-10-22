/*
 * A319207  a(n) = 1*2*3*4*5*6 + 7*8*9*10*11*12 + 13*14*15*16*17*18 + ... +
 *          (up to n).
 * https://oeis.org/A319207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/6)} (6*i)!/(6*i-6)! + * Sum_{j=1..5}
 *            (1-sign((n-j) mod 6)) * * (Product_{i=1..j} n-i+1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 41, u64 n <= 965,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // number of complete blocks of size 6
    idx_t blocks = n / 6;
    // remaining numbers that do not form a complete block
    idx_t r = n % 6;

    seq_t sum = 0;
    idx_t start = 1;               // first number of the current block

    /* Sum the products of the complete blocks */
    for (idx_t i = 0; i < blocks; ++i) {
        seq_t prod = 1;
        for (idx_t off = 0; off < 6; ++off) {
            prod *= (seq_t)(start + off);
        }
        sum += prod;
        start += 6;
    }

    /* Add the product of the remaining tail numbers,
       if any.  For r == 0 we add nothing. */
    if (r > 0) {
        seq_t prod = 1;
        for (idx_t j = 0; j < r; ++j) {
            prod *= (seq_t)(start + j);
        }
        sum += prod;
    }

    return sum;
}
