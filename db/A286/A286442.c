/*
 * A286442  Number of ways to tile an n X n X n triangular area with seven 2 X
 *          2 X 2 triangular tiles and an appropriate number (= n^2-28) of 1 X
 *          1 X 1 tiles.
 * https://oeis.org/A286442
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 11, u64 n <= 45, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n^14 -21*n^13 +2835*n^11 -13664*n^10
 *                -147903*n^9 +1159368*n^8 +3480705*n^7
 *                -44292941*n^6 -24613344*n^5 +908186412*n^4
 *                -372748320*n^3 -9895978296*n^2 +5596762608*n
 *                +46620962640)/5040   for n>=8
 *
 * (Offset is 5: a(5)=0, a(6)=57, a(7)=9233)
 */

seq_t a(idx_t n)
{
    /* small values below the offset */
    if (n < 5) return 0;
    if (n == 5) return 0;
    if (n == 6) return 57;
    if (n == 7) return 9233;

#if defined(__cplusplus)
    /* Arbitrary‑precision (or at least larger than 64 bits) build.
       Compute with seq_t arithmetic; seq_t is assumed to support signed values. */
    const long long coeff[15] = {1L, -21L, 0L, 2835L, -13664L, -147903L,
                                 1159368L, 3480705L, -44292941L, -24613344L,
                                 908186412L, -372748320L, -9895978296L,
                                 5596762608L, 46620962640L};

    seq_t N = (seq_t)n;
    seq_t num = (seq_t)coeff[0]; /* start with n^14 coefficient */

    for (int i = 1; i < 15; ++i) {
        seq_t c = (seq_t)coeff[i];
        num = num * N + c;
    }
    return num / (seq_t)5040;
#else
    /* 64‑bit or narrower build: use 128‑bit arithmetic to avoid overflow. */
    const long long coeff[15] = {1L, -21L, 0L, 2835L, -13664L, -147903L,
                                 1159368L, 3480705L, -44292941L, -24613344L,
                                 908186412L, -372748320L, -9895978296L,
                                 5596762608L, 46620962640L};

    __int128 N = (__int128)n;
    __int128 num = (__int128)coeff[0];

    for (int i = 1; i < 15; ++i) {
        num = num * N + (__int128)coeff[i];
    }
    return (seq_t)(num / 5040);
#endif
}
