/*
 * A349954  a(n) is the number of extrema that result from iterating the
 *          reduced Collatz function R(k) = A139391(k) on 2n-1 to yield 1.
 * https://oeis.org/A349954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Count the number of local extrema (maxima or minima) in the
   reduced Collatz trajectory of the odd number 2n-1 until it reaches 1.
   The reduced Collatz step R(k) transforms k to the next odd value by
   repeatedly applying the Collatz rule (3k+1)/2 for odd k or k/2 for
   even k until an odd result is obtained.  The final value 1 is always
   counted as a minimum. */

static inline seq_t next_odd(seq_t x)
{
    /* If x is even, keep dividing by 2 until it becomes odd. */
    while (x % (seq_t)2 == (seq_t)0)
        x /= (seq_t)2;
    if (x == (seq_t)1)
        return (seq_t)1;  /* 1 stays 1 */
    /* x is odd > 1: apply the odd Collatz step (3x+1)/2 and reduce to odd. */
    x = (x * (seq_t)3 + (seq_t)1) / (seq_t)2;
    while (x % (seq_t)2 == (seq_t)0)
        x /= (seq_t)2;
    return x;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;      /* a(1) = 0 */

    seq_t start = (seq_t)(2 * (seq_t)n - (seq_t)1);
    seq_t prev  = start;
    seq_t cur   = next_odd(prev);

    if (cur == (seq_t)1)
        return (seq_t)1;      /* only the final minimum */

    seq_t count = (seq_t)0;

    for (;;)
    {
        seq_t next = next_odd(cur);

        /* Current value can be a maximum or a minimum. */
        if ((cur > prev && cur > next) || (cur < prev && cur < next))
            count++;

        prev = cur;
        if (next == (seq_t)1)
            break;              /* final step reached 1 */

        cur = next;
    }

    /* The final value 1 is always a minimum. */
    count++;

    return count;
}
