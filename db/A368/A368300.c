/*
 * A368300  Main diagonal of A365484: the n-th term in the trajectory of n
 *          under the A014682 map.
 * https://oeis.org/A368300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A365484(n,n-1) – the (n‑1)-th iterate of the map f(x) =
 *            x/2 if x is even (3x+1)/2 if x is odd starting from x = n
 * Range:     u8 n <= 26, u16 n <= 9934, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset is 1: a(1)=1 */
    seq_t x = (seq_t)n;
    idx_t steps = n - 1;

    /* iterate until we hit the 1↔2 cycle or run out of steps */
    while (steps && (x != (seq_t)1) && (x != (seq_t)2))
    {
        if ((x & (seq_t)1) == 0)  /* even */
            x >>= 1;
        else                       /* odd */
            x = (x * 3 + 1) >> 1;  /* (3x+1)/2 */
        --steps;
    }

    /* if we ended early, we are in the 1↔2 loop */
    if (steps)
    {
        if ((steps & (idx_t)1) == 0)
            return x;                  /* same value after even number of steps */
        return (x == (seq_t)1) ? (seq_t)2 : (seq_t)1;  /* flip the cycle */
    }

    return x;
}
