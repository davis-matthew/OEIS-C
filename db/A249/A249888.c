/*
 * A249888  Number of length 5+6 0..n arrays with no seven consecutive terms
 *          having the maximum of any three terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A249888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2520*n^11 - 1410*n^10 + 16384*n^9 - 201*n^8 + 34052*n^7
 *            * + 7308*n^6 + 6902*n^5 + 33561*n^4 - 19142*n^3 * + 4842*n^2 +
 *            3384*n) / 2520
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not part of the OEIS range, but safe */

#ifdef __SIZEOF_INT128__
    /* When seq_t is 64‑bit, use 128‑bit intermediates to avoid overflow. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pow[12];
        unsigned __int128 bign = (unsigned __int128)n;
        pow[0] = (unsigned __int128)1;
        for (idx_t i = 1; i <= 11; ++i) pow[i] = pow[i-1] * bign;

        unsigned __int128 num = pow[11] * (unsigned __int128)2520
                              - pow[10] * (unsigned __int128)1410
                              + pow[9]  * (unsigned __int128)16384
                              - pow[8]  * (unsigned __int128)201
                              + pow[7]  * (unsigned __int128)34052
                              + pow[6]  * (unsigned __int128)7308
                              + pow[5]  * (unsigned __int128)6902
                              + pow[4]  * (unsigned __int128)33561
                              - pow[3]  * (unsigned __int128)19142
                              + pow[2]  * (unsigned __int128)4842
                              + pow[1]  * (unsigned __int128)3384;

        return (seq_t)(num / (unsigned __int128)2520);
    }
#endif

    /* Generic path that works with arbitrary‑precision seq_t types. */
    seq_t pow[12];
    pow[0] = (seq_t)1;
    for (idx_t i = 1; i <= 11; ++i) pow[i] = pow[i-1] * (seq_t)n;

    seq_t num = pow[11] * (seq_t)2520
              - pow[10] * (seq_t)1410
              + pow[9]  * (seq_t)16384
              - pow[8]  * (seq_t)201
              + pow[7]  * (seq_t)34052
              + pow[6]  * (seq_t)7308
              + pow[5]  * (seq_t)6902
              + pow[4]  * (seq_t)33561
              - pow[3]  * (seq_t)19142
              + pow[2]  * (seq_t)4842
              + pow[1]  * (seq_t)3384;

    return num / (seq_t)2520;
}
