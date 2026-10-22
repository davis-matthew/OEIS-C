/*
 * A329115  a(n) = floor(A026549(n)/5).
 * https://oeis.org/A329115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0, a(2) = 0 a(n+1) = 2*a(n) if n is odd 3*a(n)+1 if n is
 *            even
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) and a(2) are 0, so start from a(1)=0 */
    seq_t val = (seq_t)0;

    /* No need for memoisation – recompute each time.
       This is O(n) and fast enough for n up to 10 000
       and works for any seq_t (primitive or arbitrary‑precision). */
    for (idx_t i = 1; i < n; ++i) {
        if (i & (idx_t)1) {          /* i is odd */
            val = val * (seq_t)2;
        } else {                     /* i is even */
            val = val * (seq_t)3 + (seq_t)1;
        }
    }
    return val;
}
