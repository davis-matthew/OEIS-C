/*
 * A363158  Expansion of Sum_{k>0} k * x^(2*k) / (1 - x^(5*k-2)).
 * https://oeis.org/A363158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{⌊n/2⌋} k · [ (n−2k) % (5k−2) == 0 ]
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The summand contributes k whenever the exponent n can be written as
   (5k−2)·m + 2k with m ≥ 0; i.e. when (n−2k) is a multiple of (5k−2). */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; 2 * k <= n; ++k) {
        idx_t diff  = n - 2 * k;              /* n - 2k */
        idx_t denom = 5 * k - 2;              /* 5k - 2 */
        if (diff % denom == 0) {              /* divisor condition */
            total += (seq_t)k;                /* add contribution k */
        }
    }
    return total;
}
