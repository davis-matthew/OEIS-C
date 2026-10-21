/*
 * A289227  Number of ways to select 6 disjoint point triples from an n X n X
 *          n triangular point grid, each point triple forming a 2 X 2 X 2
 *          triangle.
 * https://oeis.org/A289227
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^12 -12*n^11 -129*n^10 +2090*n^9 +3985*n^8 *
 *            -142832*n^7 +152809*n^6 +4752598*n^5 -12392266*n^4 *
 *            -76011076*n^3 +274393360*n^2 +455879232*n -2015187840)/720 * for
 *            n >= 6. a(5) = 0
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 13, u64 n <= 70, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for the offset term (and any n < 5) */
    if (n < 5) return (seq_t)0;
    if (n == 5) return (seq_t)0;

#ifdef __cplusplus
    /* C++ compilation: distinguish between integral and big‑int types */
    if constexpr (std::is_integral<seq_t>::value)
    {
        /* Use 128‑bit arithmetic for intermediate values */
        typedef __uint128_t int128;
        int128 N = (int128)n;
        int128 i1 = N;
        int128 i2 = i1 * N;
        int128 i3 = i2 * N;
        int128 i4 = i3 * N;
        int128 i5 = i4 * N;
        int128 i6 = i5 * N;
        int128 i7 = i6 * N;
        int128 i8 = i7 * N;
        int128 i9 = i8 * N;
        int128 i10 = i9 * N;
        int128 i11 = i10 * N;
        int128 i12 = i11 * N;

        int128 num = i12
                    - (int128)12   * i11
                    - (int128)129  * i10
                    + (int128)2090 * i9
                    + (int128)3985 * i8
                    - (int128)142832 * i7
                    + (int128)152809 * i6
                    + (int128)4752598 * i5
                    - (int128)12392266 * i4
                    - (int128)76011076 * i3
                    + (int128)274393360 * i2
                    + (int128)455879232 * i1
                    - (int128)2015187840;

        return (seq_t)(num / 720);
    }
    else
    {
        /* seq_t is a big‑int type: use its own arithmetic */
        seq_t N = n;
        seq_t i1 = N;
        seq_t i2 = i1 * N;
        seq_t i3 = i2 * N;
        seq_t i4 = i3 * N;
        seq_t i5 = i4 * N;
        seq_t i6 = i5 * N;
        seq_t i7 = i6 * N;
        seq_t i8 = i7 * N;
        seq_t i9 = i8 * N;
        seq_t i10 = i9 * N;
        seq_t i11 = i10 * N;
        seq_t i12 = i11 * N;

        seq_t num = i12
                  - (seq_t)12    * i11
                  - (seq_t)129   * i10
                  + (seq_t)2090  * i9
                  + (seq_t)3985  * i8
                  - (seq_t)142832 * i7
                  + (seq_t)152809 * i6
                  + (seq_t)4752598 * i5
                  - (seq_t)12392266 * i4
                  - (seq_t)76011076 * i3
                  + (seq_t)274393360 * i2
                  + (seq_t)455879232 * i1
                  - (seq_t)2015187840;

        return num / 720;
    }
#else
    /* C compilation: seq_t is an unsigned integer type, use 128‑bit intermediate */
    typedef __uint128_t int128;
    int128 N = (int128)n;
    int128 i1 = N;
    int128 i2 = i1 * N;
    int128 i3 = i2 * N;
    int128 i4 = i3 * N;
    int128 i5 = i4 * N;
    int128 i6 = i5 * N;
    int128 i7 = i6 * N;
    int128 i8 = i7 * N;
    int128 i9 = i8 * N;
    int128 i10 = i9 * N;
    int128 i11 = i10 * N;
    int128 i12 = i11 * N;

    int128 num = i12
                - (int128)12   * i11
                - (int128)129  * i10
                + (int128)2090 * i9
                + (int128)3985 * i8
                - (int128)142832 * i7
                + (int128)152809 * i6
                + (int128)4752598 * i5
                - (int128)12392266 * i4
                - (int128)76011076 * i3
                + (int128)274393360 * i2
                + (int128)455879232 * i1
                - (int128)2015187840;

    return (seq_t)(num / 720);
#endif
}
