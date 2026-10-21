/*
 * A296997  Number of ways to place 3 points on an n X n point grid so that no
 *          point is equally distant from two other points on the same row or
 *          the same column.
 * https://oeis.org/A296997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 - 3*n^4 - 3*n^3 + 8*n^2)/6 - (n%2)*n/2 (OEIS
 *            A296997, offset 1). Computes the value for any n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 54, u64 n <= 256, u128 n <= 256,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 1, but return 0 for safety. */
    if (n == 0) return 0;

    /* Built‑in unsigned integer types (≤ 64 bit): use an
       intermediate __int128 for safety.  For arbitrary‑precise
       seq_t types (larger than 8 bytes) rely on seq_t arithmetic. */
    if (sizeof(seq_t) <= 8) {
        unsigned long long nn = (unsigned long long)n;
        __int128 nn128 = nn;
        __int128 n2 = nn128 * nn128;          /* n^2 */
        __int128 n3 = n2 * nn128;             /* n^3 */
        __int128 n4 = n3 * nn128;             /* n^4 */
        __int128 n6 = n3 * n3;                /* n^6 */
        __int128 num = n6 - 3 * n4 - 3 * n3 + 8 * n2;
        __int128 val = num / 6;
        if (n & 1) val -= nn128 / 2;          /* subtract ⌊n/2⌋ if n odd */
        return (seq_t)val;
    } else {
        seq_t n_seq = (seq_t)n;
        seq_t n2 = n_seq * n_seq;             /* n^2 */
        seq_t n3 = n2 * n_seq;                /* n^3 */
        seq_t n4 = n3 * n_seq;                /* n^4 */
        seq_t n6 = n3 * n3;                   /* n^6 */
        seq_t num = n6 - 3 * n4 - 3 * n3 + 8 * n2;
        seq_t val = num / 6;
        if (n & 1) val -= n_seq / 2;          /* subtract ⌊n/2⌋ if n odd */
        return val;
    }
}
