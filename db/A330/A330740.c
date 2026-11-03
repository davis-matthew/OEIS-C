/*
 * A330740  a(n) = min(n, A004488(n)), where A004488(n) is base-3 sum n+n
 *          without carries.
 * https://oeis.org/A330740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min(n, A004488(n))
 * Range:     u8 n <= 254, u16 n <= 59048, u32 n <= 59048, u64 n <= 59048,
 *            u128 n <= 59048, big any n
 * Validated: standard, pass, 98 terms through n = 9476 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A004488(n) is the base‑3 sum n+n without carries, i.e. each ternary
 * digit d is replaced by (2*d mod 3) and the result is interpreted in
 * base 3.  */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* compute r = A004488(n) */
    seq_t r = 0;
    seq_t pow3 = 1;          /* 3^i for current digit position */
    idx_t t = n;
    while (t > 0) {
        unsigned d = (unsigned)(t % 3);    /* current ternary digit 0..2 */
        t /= 3;
        unsigned fd = (d == 0 ? 0 : (d == 1 ? 2 : 1));  /* f(d) = 2*d mod 3 */
        r += pow3 * (seq_t)fd;      /* add digit contribution */
        pow3 *= (seq_t)3;           /* advance to next power of 3 */
    }

    seq_t ns = (seq_t)n; /* cast index to seq_t for comparison */
    return (ns < r) ? ns : r;
}
