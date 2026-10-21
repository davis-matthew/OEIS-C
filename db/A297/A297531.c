/*
 * A297531  Subword complexity (number of distinct blocks) of length n
 *          occurring in the "twisted" Thue-Morse sequence.
 * https://oeis.org/A297531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 2 a(2) = 4 a(3) = 6 a(4) = 10 For n ≥ 5 let m =
 *            n – 1. Set i = ⌊log₂ m⌋ and p = 2ⁱ. Then if m ≤ 3·p/2 a(n) = 4·m
 *            – 3·p/4 else if m ≤ 7·p/4 a(n) = 3·m + 3·p/4 else a(n) = 2·m +
 *            5·p/2 (All operations are performed on type seq_t; m is an
 *            idx_t.)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t _floor_log2_u64(idx_t x) {
    /* assumes x > 0 */
    idx_t r = 0;
    while (x >>= 1) ++r;
    return r;
}

seq_t a(idx_t n) {
    /* initial small values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)10;

    /* n ≥ 5 */
    idx_t m = n - 1;                     /* m = n - 1 */
    idx_t i = _floor_log2_u64(m);        /* floor(log₂ m) */
    seq_t pow2i = (seq_t)1 << i;         /* 2ⁱ */

    seq_t m_seq = (seq_t)m;
    seq_t mid1   = 3 * (pow2i >> 1);     /* 3·p/2 */
    seq_t mid2   = 7 * (pow2i >> 2);     /* 7·p/4 */

    if (m_seq <= mid1) {
        return 4 * m_seq - 3 * (pow2i >> 2);   /* 4m – 3·p/4 */
    } else if (m_seq <= mid2) {
        return 3 * m_seq + 3 * (pow2i >> 2);   /* 3m + 3·p/4 */
    } else {
        return 2 * m_seq + 5 * (pow2i >> 1);   /* 2m + 5·p/2 */
    }
}
