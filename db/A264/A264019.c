/*
 * A264019  Number of (4+1) X (n+1) arrays of permutations of 0..n*5+4 with
 *          each element having index change +-(.,.) 0,0 1,2 or 2,2.
 * https://oeis.org/A264019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*a(n-1) + 304*a(n-3) + 256*a(n-4) for n > 8, * with
 *            base values a(1)=1, a(2)=34, a(3)=1156, a(4)=20570, *
 *            a(5)=366025, a(6)=7320500, a(7)=146410000, a(8)=2898918000
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 1..8.  These constants are small enough
     * to fit in any unsigned type that seq_t may represent.  They
     * are stored in a static const array to avoid repeated
     * reinitialisation on successive calls. */
    static const seq_t base[8] = {
        (seq_t)1,
        (seq_t)34,
        (seq_t)1156,
        (seq_t)20570,
        (seq_t)366025,
        (seq_t)7320500,
        (seq_t)146410000,
        (seq_t)2898918000
    };

    if (n <= 8) {
        return base[n - 1];
    }

    /* Sliding window containing the last four sequence values:
     * win[0] = a(k-4), win[1] = a(k-3), win[2] = a(k-2), win[3] = a(k-1).
     * Initialise it with the known values for k = 9. */
    seq_t win[4];
    win[0] = base[4];   /* a5 */
    win[1] = base[5];   /* a6 */
    win[2] = base[6];   /* a7 */
    win[3] = base[7];   /* a8 */

    /* Iterate from k = 9 up to the requested n, updating the window
     * each time.  The recurrence uses the values stored in the
     * window, so no additional memory is required. */
    for (idx_t k = 9; k <= n; ++k) {
        seq_t val = 19 * win[3] + 304 * win[1] + 256 * win[0];
        win[0] = win[1];
        win[1] = win[2];
        win[2] = win[3];
        win[3] = val;
    }

    return win[3];
}
