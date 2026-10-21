/*
 * A288138  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 427", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 8*a(n-3) + 4*a(n-4) - 8*a(n-5) - 4*a(n-6) -
 *            32*a(n-7) + 32*a(n-9) with initial values a(0)=1, a(1)=3,
 *            a(2)=6, a(3)=7, a(4)=28, a(5)=63, a(6)=56, a(7)=223, a(8)=496
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[9] = {
        (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)7,
        (seq_t)28, (seq_t)63, (seq_t)56, (seq_t)223, (seq_t)496
    };

    if (n < 9)
        return init[n];

    seq_t buf[9];
    for (int i = 0; i < 9; ++i) buf[i] = init[i];

    for (idx_t i = 9; i <= n; ++i) {
        seq_t next = buf[(i - 2) % 9];
        next += buf[(i - 3) % 9] * (seq_t)8;
        next += buf[(i - 4) % 9] * (seq_t)4;
        next -= buf[(i - 5) % 9] * (seq_t)8;
        next -= buf[(i - 6) % 9] * (seq_t)4;
        next -= buf[(i - 7) % 9] * (seq_t)32;
        next += buf[(i - 9) % 9] * (seq_t)32;
        buf[i % 9] = next;
    }

    return buf[n % 9];
}
