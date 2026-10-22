/*
 * A378727  The total number of fires in a rooted undirected infinite 4-ary
 *          tree with a self-loop at the root, when the chip-firing process
 *          starts with (4^n-1)/3 chips at the root.
 * https://oeis.org/A378727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((3*n - 5)*4^n + 3*n + 5)/27
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offsets a(0)=0, a(1)=0.  For all other n the formula holds. */
    if (n == 0 || n == 1) return 0;

    /* Use signed 128‑bit arithmetic when seq_t is a native integer type
       (<= 8 bytes).  For larger (arbitrary‑precision) types we rely on
       seq_t’s own operators. */
    if (sizeof(seq_t) <= 8) {
        __int128 pow = 1;
        for (idx_t i = 0; i < n; ++i) pow *= 4;
        __int128 num = (__int128)(3 * n - 5) * pow + (__int128)(3 * n + 5);
        __int128 res = num / 27;
        return (seq_t)res;
    } else {
        seq_t pow = 1;
        for (idx_t i = 0; i < n; ++i) pow *= 4;
        seq_t term1 = ((seq_t)(3 * n) - (seq_t)5) * pow;
        seq_t num   = term1 + ((seq_t)(3 * n) + (seq_t)5);
        return num / (seq_t)27;
    }
}
