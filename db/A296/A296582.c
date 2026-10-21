/*
 * A296582  Number of n X 2 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 0, 2 or 3 neighboring 1s.
 * https://oeis.org/A296582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) * - a(n-3) - 8*a(n-4) - 3*a(n-6) * -
 *            2*a(n-7) - 5*a(n-8) + a(n-9) * (a(1)=3, a(2)=9, a(3)=22,
 *            a(4)=62, a(5)=172, a(6)=480, * a(7)=1358, a(8)=3849, a(9)=10951)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A296582 for any n >= 1.
 * The implementation uses only the first nine terms as seeds
 * and iterates the recurrence, keeping the last nine values in a
 * fixed-size buffer.  No dynamic memory allocation or global state
 * is required, so the function works correctly for any call order. */

seq_t a(idx_t n)
{
    /* first 9 terms, indexed 0..8 for a(1)..a(9) */
    static const seq_t init[9] = {
        (seq_t)3,   (seq_t)9,   (seq_t)22,  (seq_t)62,
        (seq_t)172, (seq_t)480, (seq_t)1358,(seq_t)3849,(seq_t)10951
    };

    if (n <= 9) return init[n-1];

    seq_t last[9];
    for (idx_t i = 0; i < 9; ++i) last[i] = init[i];

    seq_t next;
    for (idx_t k = 10; k <= n; ++k) {
        next = last[8]*3 + last[7] - last[6] - last[5]*8
             - last[3]*3 - last[2]*2 - last[1]*5 + last[0];
        for (idx_t j = 0; j < 8; ++j) last[j] = last[j+1];
        last[8] = next;
    }
    return last[8];
}
