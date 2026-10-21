/*
 * A276089  Factorial base representation of n is aerated (and then converted
 *          back to decimal): for n = sum_{i=1..} digit(i)*i! (with each
 *          digit(i) <= i), a(n) = sum_{i=1..} digit(i)*(2i-1)!.
 * https://oeis.org/A276089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i≥1} d(i)·(2i−1)! where n = Σ_{i≥1} d(i)·i! (factorial
 *            base)
 * Range:     u8 n <= 17, u16 n <= 119, u32 n <= 5039, u64 n <= 5039,
 *            u128 n <= 5039, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n in factorial base: n = Σ d(i)*i!   d(i) in [0,i] */
    seq_t a_val = 0;                /* result a(n) */
    seq_t fact  = 1;                /* factorial(1) = 1!    */
    idx_t tmp   = n;                /* remaining part of n   */
    idx_t i     = 1;                /* position in factorial base */

    while (tmp > 0) {
        /* digit d at position i (using base i+1) */
        idx_t d = tmp % (i + 1);    /* 0 ≤ d ≤ i */
        tmp  /= (i + 1);

        a_val += (seq_t)d * fact;  /* add d·(2i−1)! */

        /* prepare factorial for next odd index (2(i+1)−1) = 2i+1 */
        /* factorial(2i+1) = factorial(2i−1) · (2i) · (2i+1) */
        fact *= (seq_t)(2 * i) * (seq_t)(2 * i + 1);

        ++i;
    }
    return a_val;
}
