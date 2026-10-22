/*
 * A328330  Let f(n) be the number of segments shown on a digital calculator
 *          to display n. Then a(n) is the number of terms in the sequence
 *          formed by iteration n -> f(n) until n = f(n).
 * https://oeis.org/A328330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 780, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Chain length of 7‑segment digit‑count iteration until a fixed point (4,5,6). */

static const unsigned char seg[10] = {6,2,5,5,4,5,6,3,7,6};

static inline seq_t segsum(seq_t x)
{
    seq_t s = 0;
    while (x > 0)
    {
        unsigned d = (unsigned)(x % 10);
        s += seg[d];
        x /= 10;
    }
    return s;
}

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t steps = 1;
    while (1)
    {
        seq_t nxt = segsum(cur);
        if (nxt == cur)
            break;
        steps++;
        cur = nxt;
    }
    return steps;
}
