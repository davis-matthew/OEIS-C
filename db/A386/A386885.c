/*
 * A386885  a(n) is the number of iterations that -n requires to reach 1 under
 *          the map x -> -x/2 if x is even, 3x + 1 if x is odd.
 * https://oeis.org/A386885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2540, u16 n <= 9827, u32 n <= 9827, u64 n <= 9827,
 *            u128 n <= 9827, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of iterations that n requires to reach -1 under the map
 *              x even -> -x/2
 *              x odd  -> 3x-1
 * (equivalently, the number of steps for -n to reach 1 using
 * x even -> -x/2, x odd -> 3x+1).  The recurrence is
 *    a(1) = 2
 *    a(n) = 1 + a(f(n))  for n > 1
 * where f(n) = -n/2 if n is even, 3n-1 if n is odd.  */

seq_t a(idx_t n)
{
    /* Use a signed 64‑bit variable to hold intermediate values; the
     * sequence is defined only for n ≥ 1 and intermediate values
     * never exceed 64‑bit for the test range (≤ 5×10⁷). */
    long long x = (long long)n;
    seq_t steps = 0;

    /* iterate until the map reaches -1 */
    while (x != -1) {
        if ((x & 1LL) == 0) {          /* x even: x = -x/2 */
            x = -x / 2;
        } else {                      /* x odd:  x = 3x - 1 */
            x = 3 * x - 1;
        }
        steps++;
    }

    return steps;
}
