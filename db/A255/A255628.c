/*
 * A255628  Number of length n+6 0..2 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 47, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A255628: Number of length n+6 0..2 arrays with at most two downsteps in
 * every n consecutive neighbor pairs.
 *
 * The sequence is known (empirically) to be a polynomial of degree 8 for
 * n > 4:
 *
 *   a(n) = (1/40320)*n^8 + (1/480)*n^7 + (13/192)*n^6 + (67/12)*n^5
 *        + (48663/640)*n^4 + (46699/480)*n^3 + (5325055/2016)*n^2
 *        - (53947/8)*n + 3866        (n > 4)
 *
 * The polynomial has denominator 40320.  For n ≤ 4 the initial terms are
 * given explicitly.
 *
 * The implementation evaluates the polynomial using 128‑bit arithmetic
 * (signed __int128) to avoid overflow for the 64‑bit build when n is
 * large enough that the numerator does not fit in 64 bits.  The final
 * division result is converted to seq_t; if the value does not fit the
 * conversion behaviour is defined by the chosen integer type (arbitrary‑
 * precision classes have a suitable constructor).
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the first four terms explicitly. */
    if (n == 1) return (seq_t)2187;
    if (n == 2) return (seq_t)6561;
    if (n == 3) return (seq_t)19683;
    if (n == 4) return (seq_t)51768;

    /* For n > 4 use the polynomial form. */
    /* Convert index to a larger signed type for intermediate arithmetic. */
    signed __int128 N = (signed __int128)((seq_t)n);

    /* Powers of N. */
    signed __int128 n2 = N * N;          /* n^2 */
    signed __int128 n3 = n2 * N;         /* n^3 */
    signed __int128 n4 = n3 * N;         /* n^4 */
    signed __int128 n5 = n4 * N;         /* n^5 */
    signed __int128 n6 = n5 * N;         /* n^6 */
    signed __int128 n7 = n6 * N;         /* n^7 */
    signed __int128 n8 = n7 * N;         /* n^8 */

    /* Numerator of the polynomial with common denominator 40320. */
    signed __int128 num = n8;
    num += (signed __int128)84      * n7;
    num += (signed __int128)2730    * n6;
    num += (signed __int128)225120  * n5;
    num += (signed __int128)3065769 * n4;
    num += (signed __int128)3922716 * n3;
    num += (signed __int128)106501100 * n2;
    num -= (signed __int128)271892880 * N;
    num += (signed __int128)155877120;

    /* Final division by 40320 gives the integer sequence value. */
    seq_t result = (seq_t)(num / 40320);
    return result;
}
