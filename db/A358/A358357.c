/*
 * A358357  Maximum length of an induced path (or chordless path) in the
 *          n-folded cube graph.
 * https://oeis.org/A358357
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A358357: maximum length of an induced path in the n‑folded cube
   a(2)=1, a(3)=1, a(4)=2; for n>=5: a(n)=3*2^(n-4)-2. */
seq_t a(idx_t n)
{
    if (n == 2 || n == 3) return (seq_t)1;
    if (n == 4)           return (seq_t)2;
    if (n < 2)            return (seq_t)0;   /* out of range, harmless */

    seq_t v = (seq_t)1;          /* 2^0 */
    v <<= (n - 4);               /* v = 2^(n-4) */
    v *= 3;                      /* v = 3*2^(n-4) */
    v -= 2;                      /* v = 3*2^(n-4) - 2 */
    return v;
}
