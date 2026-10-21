/*
 * A248924  Sequence derived from arithmetic relations between powers of phi
 *          (A001622): a(n) = phi^n - (-1)^n * (n - phi^-n).
 * https://oeis.org/A248924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n) - (-1)^n * n Lucas(n) is the nth Lucas number
 *            (L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2))
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for Lucas numbers */
    static seq_t *lucas = NULL;     /* stored numerator: L(i)   */
    static size_t    cap   = 0;      /* number of elements cached */

    /* initialise cache for the first two Lucas numbers */
    if (lucas == NULL) {
        lucas = (seq_t *)malloc(sizeof(seq_t) * 2);
        lucas[0] = (seq_t)2;   /* L(0) */
        lucas[1] = (seq_t)1;   /* L(1) */
        cap = 2;
    }

    /* grow cache up to n if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        lucas = (seq_t *)realloc(lucas, sizeof(seq_t) * new_cap);
        for (size_t i = cap; i <= (size_t)n; ++i) {
            /* L(i) = L(i-1) + L(i-2) */
            lucas[i] = lucas[i - 1] + lucas[i - 2];
        }
        cap = new_cap;
    }

    /* compute a(n) = L(n) - (-1)^n * n
       For even n: (-1)^n = 1, so subtract n
       For odd n:  (-1)^n = -1, so add n   */
    seq_t nn = (seq_t)n;
    if ((n & 1ULL) == 0) {
        /* even n */
        return lucas[(size_t)n] - nn;
    } else {
        /* odd n */
        return lucas[(size_t)n] + nn;
    }
}
