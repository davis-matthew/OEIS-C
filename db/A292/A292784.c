/*
 * A292784  a(n) = n! * [x^n] 1/sqrt(1 - 2*n*x).
 * https://oeis.org/A292784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)! * n^n / (n! * 2^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t limit = 2 * n;
    bool *is_composite = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_composite)
        return (seq_t)0; /* allocation failure */

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
        }
    }

    seq_t res = 1;
    for (idx_t p = 2; p <= limit; ++p) {
        if (is_composite[p])
            continue; /* not prime */

        /* exponent of prime p in a(n) */
        idx_t exp = 0;

        /* v_p((2n)!) */
        idx_t t = limit;
        while (t) {
            t /= p;
            exp += t;
        }

        /* subtract v_p(n!) */
        t = n;
        while (t) {
            t /= p;
            exp -= t;
        }

        /* add n * v_p(n) */
        t = n;
        idx_t vp = 0;
        while (t && t % p == 0) {
            t /= p;
            vp++;
        }
        exp += n * vp;

        /* divide by 2^n if p == 2 */
        if (p == 2)
            exp -= n;

        /* multiply res by p^exp */
        while (exp--)
            res *= (seq_t)p;
    }

    free(is_composite);
    return res;
}
