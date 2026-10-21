/*
 * A244878  Number of 6 X 6 traceless symmetric magic squares with magic sum
 *          n.
 * https://oeis.org/A244878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (945*(507+5*(-1)^n) + 1480896*n + 2062800*n^2 + 1747040*n^3 +
 *            989100*n^4 + 383628*n^5 + 100800*n^6 + 17160*n^7 + 1710*n^8 +
 *            76*n^9) / 483840
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 363, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Choose the parity-dependent constant 945*(507+5*(-1)^n) */
    /* 945*512 = 483840 for even n, 945*502 = 474390 for odd n */
    unsigned long long const_part;
    if ((n & 1) == 0) {
        const_part = 483840ULL;
    } else {
        const_part = 474390ULL;
    }

    /* Use 128‑bit intermediate to avoid overflow for 64‑bit builds,
       while still compiling on systems that only support 64‑bit types. */
    unsigned __int128 res = 76;
    unsigned __int128 nn  = (unsigned __int128)n;

    res = res * nn + 1710;
    res = res * nn + 17160;
    res = res * nn + 100800;
    res = res * nn + 383628;
    res = res * nn + 989100;
    res = res * nn + 1747040;
    res = res * nn + 2062800;
    res = res * nn + 1480896;
    res = res * nn + (unsigned __int128)const_part;
    res = res / 483840;

    return (seq_t)res;
}
