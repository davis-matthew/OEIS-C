/*
 * A281660  The least common multiple of 1+n and 1+n^2.
 * https://oeis.org/A281660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(1+n,1+n^2) = (1+n)*(1+n^2)/gcd(1+n,1+n^2)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t inc  = (seq_t)n + 1;
    seq_t inc2 = (seq_t)n * (seq_t)n + 1;
    seq_t g = gcd_seq(inc, inc2);
    seq_t num = inc * inc2;
    return num / g;
}
