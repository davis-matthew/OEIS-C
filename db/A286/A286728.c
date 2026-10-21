/*
 * A286728  Positions of 1 in A286063; complement of A286727.
 * https://oeis.org/A286728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n – 2*t(n‑1) * where t(k) is the Thue‑Morse sequence:
 *            t(k) = 0 if popcount(k) is even, * and t(k) = 1 if popcount(k)
 *            is odd. Thus the term a(n) is 3*n when * t(n‑1)=0, and 3*n–2
 *            when t(n‑1)=1. This reproduces the sequence * A286728
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute parity of popcount(n‑1), i.e. the Thue‑Morse value. */
    idx_t k = n - 1;          /* n >= 1, so k is unsigned and safe. */
    unsigned int t = 0;       /* 0 or 1 */
    while (k) {
        t ^= 1;        /* flip parity on each set bit */
        k &= k - 1;    /* drop lowest set bit */
    }

    /* a(n) = 3*n – 2*t */
    seq_t val = (seq_t)n * (seq_t)3;  /* 3*n */
    if (t) val -= (seq_t)2;
    return val;
}
