/*
 * A274972  Numbers x such that there exists n in N : (x+1)^3 - x^3 = 61*n^2.
 * https://oeis.org/A274972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A274972: a(n) = 975*a(n-1) - 975*a(n-2) + a(n-3) for n > 3
   a(1) = 4, a(2) = 4387, a(3) = 4273420
*/
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined for n = 0 */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)4387;
    if (n == 3) return (seq_t)4273420;

    seq_t a_nm3 = (seq_t)4;            /* a(n-3) = a(1) */
    seq_t a_nm2 = (seq_t)4387;         /* a(n-2) = a(2) */
    seq_t a_nm1 = (seq_t)4273420;      /* a(n-1) = a(3) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)975 * a_nm1 - (seq_t)975 * a_nm2 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
