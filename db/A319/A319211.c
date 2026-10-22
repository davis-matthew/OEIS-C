/*
 * A319211  a(n) = 1*2*3*4*5*6*7*8*9 + 10*11*12*13*14*15*16*17*18 +
 *          19*20*21*22*23*24*25*26*27 + ... + (up to n).
 * https://oeis.org/A319211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/9)} (9*i)!/(9*i-9)! + Sum_{j=1..8}
 *            (1-sign((n-j) mod 9)) * (Product_{i=1..j} n-i+1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 134, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It computes the sum of the products of consecutive blocks of 9
   integers, plus the product of any remaining trailing integers. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; not used but harmless */

    seq_t sum  = 0;
    seq_t prod = 1;

    for (idx_t i = 1; i <= n; ++i) {
        prod *= (seq_t)i;          /* multiply by current integer */
        if (i % 9 == 0) {          /* end of a block of 9 numbers */
            sum += prod;           /* add block product to the sum */
            prod = 1;              /* reset for next block */
        }
    }

    /* If n is not a multiple of 9, prod now holds the product of the last
       incomplete block (the remainder). Add it to the sum. */
    if (n % 9 != 0) {
        sum += prod;
    }

    return sum;
}
