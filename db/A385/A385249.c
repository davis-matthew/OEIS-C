/*
 * A385249  Number of iterations of seven segments count x -> A010371(x) to go
 *          from n to a fixed point.
 * https://oeis.org/A385249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of iterations of the seven‑segment count (A010371)
 *            until reaching the fixed points 4, 5, or 6
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned char seg[10] = {6,2,5,5,4,5,6,4,7,6};

static inline seq_t segcount(seq_t x)
{
    if (x == 0)
        return (seq_t)seg[0];

    seq_t sum = 0;
    while (x > 0)
    {
        unsigned d = (unsigned)(x % (seq_t)10);
        sum += (seq_t)seg[d];
        x /= (seq_t)10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n;
    seq_t steps = 0;

    while (v != (seq_t)4 && v != (seq_t)5 && v != (seq_t)6)
    {
        v = segcount(v);
        ++steps;
    }

    return steps;
}
