/*
 * A258644  Fourth arithmetic derivative of n.
 * https://oeis.org/A258644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A003415^4(n) – fourth arithmetic derivative of n
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t derivative(seq_t m)
{
    if (m <= 1)
        return 0;

    seq_t orig = m;
    seq_t t = m;
    seq_t res = 0;

    /* factor 2 */
    if ((t % 2) == 0) {
        unsigned int exp = 0;
        while ((t % 2) == 0) {
            t /= 2;
            exp++;
        }
        res += (orig / 2) * exp;
    }

    /* odd prime factors */
    for (seq_t p = 3; p <= t / p; p += 2) {
        if ((t % p) == 0) {
            unsigned int exp = 0;
            while ((t % p) == 0) {
                t /= p;
                exp++;
            }
            res += (orig / p) * exp;
        }
    }

    /* remaining prime factor */
    if (t > 1) {
        res += orig / t;
    }

    return res;
}

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n;
    for (int i = 0; i < 4; ++i) {
        v = derivative(v);
    }
    return v;
}
