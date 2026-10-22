/*
 * A334356  Number of nonequivalent proper colorings of the vertices of a cube
 *          using at most n colors up to rotations and reflections of the
 *          cube.
 * https://oeis.org/A334356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n - 1)*(n^6 - 11*n^5 + 61*n^4 - 195*n^3 + 384*n^2 -
 *            428*n + 216)/48
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 27, u64 n <= 416, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;         /* a(1)=0, and values for 0 are irrelevant */

#ifdef __SIZEOF_INT128__
    /* 128‑bit temporary arithmetic guarantees no overflow for n<=10000. */
    unsigned __int128 nn   = n;          /* n */
    unsigned __int128 nn2  = nn * nn;    /* n^2 */
    unsigned __int128 nn3  = nn2 * nn;   /* n^3 */
    unsigned __int128 nn4  = nn3 * nn;   /* n^4 */
    unsigned __int128 nn5  = nn4 * nn;   /* n^5 */
    unsigned __int128 nn6  = nn5 * nn;   /* n^6 */

    unsigned __int128 poly =
        nn6
        - (unsigned __int128)11 * nn5
        + (unsigned __int128)61  * nn4
        - (unsigned __int128)195 * nn3
        + (unsigned __int128)384 * nn2
        - (unsigned __int128)428 * nn
        + (unsigned __int128)216;

    unsigned __int128 prod = nn * (nn - 1) * poly / (unsigned __int128)48;
    return (seq_t)prod;
#else
    /* Fallback to seq_t arithmetic.  This is safe when seq_t is at least
       128‑bits or an arbitrary‑precision type.  For tiny seq_t types it
       will only give correct results when the final value fits in seq_t. */
    seq_t nn  = n;
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;
    seq_t nn4 = nn3 * nn;
    seq_t nn5 = nn4 * nn;
    seq_t nn6 = nn5 * nn;

    seq_t poly = nn6
        - (seq_t)11 * nn5
        + (seq_t)61  * nn4
        - (seq_t)195 * nn3
        + (seq_t)384 * nn2
        - (seq_t)428 * nn
        + (seq_t)216;

    seq_t prod = nn * (nn - 1) * poly / (seq_t)48;
    return prod;
#endif
}
