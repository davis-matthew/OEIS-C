/*
 * A344276  Number of halving and tripling steps to reach 3 in the '3x+3'
 *          problem, or -1 if 3 is never reached.
 * https://oeis.org/A344276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(3)=0; for n>0, if n even, a(n)=a(n/2)+1; if n odd,
 *            a(n)=a(3n+3)+1
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of steps in the 3x+3 problem to reach 3.
   Returns the number of steps, or (seq_t)-1 if 3 is not reached
   within a prescribed limit (should not occur for inputs up to ~10⁴). */
seq_t a(idx_t n)
{
    if (n == 3) return 0;

    seq_t cur = (seq_t)n;
    seq_t steps = 0;

    /* we assume that all n ≤ 10k reach 3 within a modest number of steps */
    /* a safety limit to avoid infinite loops on pathological inputs */
    const seq_t LIMIT = 1ULL << 20; /* 1,048,576 */

    while (cur != 3 && steps < LIMIT) {
        if ((cur & 1) == 0) {
            cur = cur >> 1;
        } else {
            cur = 3 * cur + 3;
        }
        steps++;
    }

    if (cur != 3) {
        return (seq_t)-1; /* did not reach 3 within limit */
    }
    return steps;
}
