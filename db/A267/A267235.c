/*
 * A267235  Number of length-6 0..n arrays with no following elements greater
 *          than or equal to the first repeated value.
 * https://oeis.org/A267235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + (197/60)n^5 + 3n^4 + (3/4)n^3 - (1/30)n
 *            (equivalently a(n) = (60n^6 + 197n^5 + 180n^4 + 45n^3 - 2n) / 60
 *            ) This is the closed‑form expression derived from the generating
 *            function, which generates all terms of A267235 for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The function works for all positive n.  For the specific offset
       of the sequence (a(1) is the first term) we simply compute the
       value for the supplied index n.  All operations are performed
       using unsigned arithmetic.  To avoid overflow in the 64‑bit
       build for moderately large n, intermediate values are carried
       out in an unsigned 128‑bit integer when available.  The final
       result is cast back to seq_t.  On an arbitrary‑precision build
       the bigger type will be promoted automatically.  */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;         /* n^2 */
    unsigned __int128 n3 = n2 * nn;         /* n^3 */
    unsigned __int128 n4 = n3 * nn;         /* n^4 */
    unsigned __int128 n5 = n4 * nn;         /* n^5 */
    unsigned __int128 n6 = n5 * nn;         /* n^6 */

    unsigned __int128 num = n6 * 60;        /* 60 n^6 */
    num += n5 * 197;                       /* +197 n^5 */
    num += n4 * 180;                       /* +180 n^4 */
    num += n3 * 45;                        /* +45  n^3 */
    num -= nn * 2;                         /* - 2 n  */

    return (seq_t)(num / 60);               /* divide by 60 */
}
