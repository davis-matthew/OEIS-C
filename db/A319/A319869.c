/*
 * A319869  a(n) = 5*4*3*2*1 + 10*9*8*7*6 + 15*14*13*12*11 + ... + (up to the
 *          n-th term).
 * https://oeis.org/A319869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{m=1}^{⌊n/5⌋} Π_{i=0}^{4} (5m - i) + Π_{i=0}^{r-1}
 *            (5(⌊n/5⌋+1) - i) where r = n mod 5
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 74, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t full_blocks = n / 5;
    idx_t r = n % 5;

    seq_t sum = 0;

    for (idx_t m = 1; m <= full_blocks; ++m) {
        seq_t start = (seq_t)5 * (seq_t)m;      /* 5m  */
        seq_t prod  = 1;
        for (idx_t i = 0; i < 5; ++i)
            prod *= (seq_t)(start - (seq_t)i);
        sum += prod;
    }

    if (r > 0) {
        seq_t start = (seq_t)5 * ((seq_t)full_blocks + (seq_t)1);  /* 5*(floor(n/5)+1) */
        seq_t prod  = 1;
        for (idx_t i = 0; i < r; ++i)
            prod *= (seq_t)(start - (seq_t)i);
        sum += prod;
    }

    return sum;
}
