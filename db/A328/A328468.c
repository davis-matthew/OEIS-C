/*
 * A328468  Row 5 of A328464: a(n) = A276156(32n - 16) / 210.
 * https://oeis.org/A328468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276156(32*n-16)/210 = sum over set bits i of (2n-1) of
 *            q(i), q(0)=1, q(i)=q(i-1)*prime(i+4)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 128, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 56 terms through n = 7889 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A328468_q[64];
static int A328468_init = 0;

seq_t a(idx_t n) {
    if (!A328468_init) {
        static const unsigned pr[67] = {
            2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,
            59,61,67,71,73,79,83,89,97,101,103,107,109,113,
            127,131,137,139,149,151,157,163,167,173,179,181,
            191,193,197,199,211,223,227,229,233,239,241,251,
            257,263,269,271,277,281,283,293,307,311,313
        };
        A328468_q[0] = (seq_t)1;
        for (int i = 1; i < 64; ++i)
            A328468_q[i] = A328468_q[i-1] * (seq_t)pr[i+3];
        A328468_init = 1;
    }
    idx_t m = 2 * n - 1;
    seq_t r = (seq_t)0;
    int pos = 0;
    while (m != 0) {
        if (m & 1) r += A328468_q[pos];
        m >>= 1;
        ++pos;
    }
    return r;
}
