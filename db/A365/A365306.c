/*
 * A365306  Maximal reset threshold over all binary partial finite automata of
 *          n states.
 * https://oeis.org/A365306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=0, a(2)=1, a(3)=4 For n >= 4: a(n) = (n-1)^2 +
 *            ⌊(n-4)(n-5)/2⌋ − max(0, n-8) (All arithmetic is performed in the
 *            type seq_t)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small fixed cases */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;

    /* n >= 4 */
    seq_t n1 = (seq_t)(n - 1);
    seq_t base = n1 * n1;          /* (n-1)^2 */

    /* Compute ⌊(n-4)(n-5)/2⌋ safely avoiding negative intermediate values */
    seq_t prod = 0;
    if (n >= 6) {
        prod = (seq_t)(n - 4) * (seq_t)(n - 5);
    }
    seq_t add  = prod / 2;          /* integer division is floor for non‑negative numerator */

    seq_t result = base + add;

    /* Subtract (n-8) for n > 8 */
    if (n > 8) {
        result -= (seq_t)(n - 8);
    }

    return result;
}
