/*
 * A319206  a(n) = 1*2*3*4*5 + 6*7*8*9*10 + 11*12*13*14*15 + ... + (up to n).
 * https://oeis.org/A319206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{i=1..⌊n/5⌋} (5*i)!/(5*i-5)! + ∑_{j=1..(n mod 5)}
 *            ∏_{k=5⌊n/5⌋+1}^{5⌊n/5⌋+j} k (offset 1: a(1)=1.)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 74, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t fullBlocks = n / 5;

    /* Sum complete blocks of five consecutive integers */
    for (idx_t i = 1; i <= fullBlocks; ++i) {
        seq_t blockProd = 1;
        idx_t start = 5 * (i - 1) + 1;               /* first number of the block */
        for (idx_t k = start; k < start + 5; ++k) {  /* product of the five numbers */
            blockProd *= (seq_t)k;
        }
        result += blockProd;
    }

    /* Add the partial block (if any) */
    idx_t r = n % 5;
    if (r > 0) {
        seq_t partialProd = 1;
        idx_t start = fullBlocks * 5 + 1;          /* first number of the last partial block */
        for (idx_t k = start; k < start + r; ++k) {
            partialProd *= (seq_t)k;
        }
        result += partialProd;
    }

    return result;
}
