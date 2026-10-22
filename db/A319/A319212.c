/*
 * A319212  a(n) = 1*2*3*4*5*6*7*8*9*10 + 11*12*13*14*15*16*17*18*19*20 +
 *          21*22*23*24*25*26*27*28*29*30 + ... + (up to n).
 * https://oeis.org/A319212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 18, u64 n <= 89, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = sum_{k=1}^{⌊n/10⌋} ∏_{i=10*(k-1)+1}^{10*k} i
 *            + ∏_{i=10*⌊n/10⌋+1}^{n} i
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* sum over complete blocks of 10 */
    idx_t last_full = (n / 10) * 10;
    for (idx_t block_start = 1; block_start <= last_full; block_start += 10) {
        seq_t prod = 1;
        for (idx_t i = block_start; i < block_start + 10; ++i) {
            prod *= (seq_t)i;
        }
        sum += prod;
    }
    /* partial last block (if any) */
    idx_t rem = n % 10;
    if (rem) {
        seq_t prod = 1;
        for (idx_t i = last_full + 1; i <= n; ++i) {
            prod *= (seq_t)i;
        }
        sum += prod;
    }
    return sum;
}
