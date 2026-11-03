/*
 * A366600  a(n) = (1 + A033264(n))*a(A053645(n)) for n > 0 with a(0) = 1.
 * https://oeis.org/A366600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(2n+1) = a(n) a(4n) = a(2n) a(4n+2) = 2 * b(n>>2) b(2n+1) = 2 *
 *            b(n) b(2n) = 3 * c(n-1,1) c(2n+1,k) = c(n,k) c(4n+2,k) = (k+2) *
 *            c(2n, k) c(4n,k) = (k+3) * c(n-1, k+1) c(0,k) = 1
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 79 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Helper c */
static seq_t _c(idx_t n, unsigned k)
{
    if (n == 0)
        return (seq_t)1;

    if (n & 1)                     /* n odd */
        return _c(n >> 1, k);

    if ((n & 3) == 2)              /* n = 4m+2 */
    {
        idx_t t = (n >> 1) - 1;    /* t = 2m = n/2 - 1 */
        return (seq_t)(k + 2) * _c(t, k);
    }

    /* n % 4 == 0, i.e. n = 4m */
    idx_t m = n >> 2;              /* m = n/4 */
    return (seq_t)(k + 3) * _c(m - 1, k + 1);
}

/* Helper b */
static seq_t _b(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n & 1)                     /* odd */
        return (seq_t)2 * _b(n >> 1);

    /* even */
    idx_t m = n >> 1;              /* n = 2m  */
    return (seq_t)3 * _c(m - 1, 1);
}

/* Forward declarations of helpers */
static seq_t _b(idx_t n);
static seq_t _c(idx_t n, unsigned k);

/* Main sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n & 1)                     /* odd */
        return a(n >> 1);

    if ((n & 3) == 0)              /* n % 4 == 0 */
        return a(n >> 1);

    /* n % 4 == 2 */
    return (seq_t)2 * _b(n >> 2);
}
