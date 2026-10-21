/*
 * A285475  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 3", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1 - (-2)^n + (-1)^n + 3*2^n)/2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent closed form: a(2k) = 2^(2k), a(2k+1) = 2^(2k+2) - 1 */

static seq_t pow2seq(idx_t exp) {
    seq_t res = (seq_t)1;
    seq_t base = (seq_t)2;
    while (exp) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n) {
    if ((n & 1) == 0) {
        return pow2seq(n);
    } else {
        seq_t v = pow2seq(n + 1);
        return v - (seq_t)1;
    }
}
