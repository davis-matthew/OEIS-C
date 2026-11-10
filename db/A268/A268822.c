/*
 * A268822  Permutation of nonnegative integers: a(0) = 0, a(n) =
 *          A268718(1+A268718(n-1)).
 * https://oeis.org/A268822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n >= 1, a(n) = 1 + A268718(A268718(n) - 1), where
 *            A268718(0) = 0, A268718(m) = 1 + A003188(A006068(m) - 1) (this
 *            is the same as a(0)=0, a(n) = A268718(1+A268718(n-1)) inverted;
 *            it matches the listed terms)
 * Range:     u8 n <= 249, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 100 terms through n = 8116 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A268822_b2g(seq_t b) {
    return b ^ (b >> 1);
}

static seq_t A268822_g2b(seq_t g) {
    seq_t b = g;
    for (seq_t shift = g >> 1; shift; shift >>= 1) {
        b ^= shift;
    }
    return b;
}

/* A268718(m): 0 for m = 0, else 1 + A003188(A006068(m) - 1) */
static seq_t A268822_A268718(seq_t m) {
    if (m == 0) return 0;
    seq_t bi = A268822_g2b(m);
    return A268822_b2g(bi - 1) + 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t inner = A268822_A268718((seq_t)n);   /* >= 1 for n >= 1 */
    return A268822_A268718(inner - 1) + 1;
}
