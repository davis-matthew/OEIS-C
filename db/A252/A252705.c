/*
 * A252705  The number of ways to color the faces of a regular dodecahedron
 *          with n colors, counting mirror images as one.
 * https://oeis.org/A252705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^2+1)*(n^8-n^6+16*n^4+44)/120
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 60, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast idx_t to seq_t so that all arithmetic is in seq_t. */
    seq_t N = n;

#if defined(__SIZEOF_INT128__)
    /* If the compiler supports unsigned __int128, we do the calculation
       with 128‑bit intermediates to avoid overflow for 64‑bit seq_t.  The
       cast from seq_t to unsigned __int128 works for built‑in unsigned
       types and, for the arbitrary‑precision builds, depends on the
       library providing a constructor from uint64_t. */
    __uint128_t n128 = (unsigned __int128)N;
    __uint128_t n2   = n128 * n128;          /* n^2          */
    __uint128_t n4   = n2 * n2;              /* n^4          */
    __uint128_t n6   = n4 * n2;              /* n^6          */
    __uint128_t n8   = n4 * n4;              /* n^8          */

    __uint128_t term = n8 - n6 + (__uint128_t)16 * n4 + 44;
    __uint128_t res  = n2 * (n2 + 1);
    res = res * term;
    res = res / 120;                         /* division exact */

    return (seq_t)res;
#else
    /* Fallback without 128‑bit support: use seq_t arithmetic.  For
       arbitrary‑precision seq_t this is safe; for 64‑bit seq_t it may
       overflow, but this branch is only used when __uint128_t is not
       available.  The problem configuration will normally provide
       __uint128_t. */
    seq_t n2   = N * N;          /* n^2 */
    seq_t n4   = n2 * n2;        /* n^4 */
    seq_t n6   = n4 * n2;        /* n^6 */
    seq_t n8   = n4 * n4;        /* n^8 */

    seq_t term = n8 - n6 + (seq_t)16 * n4 + (seq_t)44;
    seq_t res  = n2 * (n2 + (seq_t)1);
    res = res * term;
    res = res / (seq_t)120;
    return res;
#endif
}
