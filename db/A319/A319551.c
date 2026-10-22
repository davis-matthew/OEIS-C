/*
 * A319551  a(n) = 1*2*3*4*5*6*7*8*9*10 - 11*12*13*14*15*16*17*18*19*20 +
 *          21*22*23*24*25*26*27*28*29*30 - ... + (up to n).
 * https://oeis.org/A319551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k=0}^{⌊(n-1)/10⌋} (-1)^k * Π_{i=10k+1}^{min(10k+10,
 *            n)} i (alternating products of consecutive blocks of length 10,
 *            terminating at n)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 18, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum   = 0;          // running sum of block products with alternating signs
    seq_t prod  = 1;          // product of the current block
    int   sign  = 1;          // +1 for even‑indexed blocks, –1 for odd‑indexed blocks

    for (idx_t i = 1; i <= n; ++i) {
        prod *= (seq_t)i;                    // extend current block product

        /* If we reach the end of the block (i%10==0) or the last element (i==n),
           add the block product (with sign) to the sum, reset for the next block. */
        if ((i % 10 == 0) || (i == n)) {
            sum += (sign > 0) ? prod : -prod;
            prod = 1;                        // reset for next block
            sign = -sign;                    // flip sign for the next block
        }
    }
    return sum;
}
