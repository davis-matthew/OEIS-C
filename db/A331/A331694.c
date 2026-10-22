/*
 * A331694  For any n > 0, let d_1, ..., d_k be the divisors of n, in
 *          ascending order; set e_0 = 0 and for i = 1..k, if e_{i-1} >= d_i
 *          then set e_i = e_{i-1} - d_i else set e_i = e_{i-1} + d_i; a(n) =
 *          e_k.
 * https://oeis.org/A331694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the final accumulator e_k after processing all divisors
 *            of n in ascending order. Start with e_0 = 0; for each divisor d:
 *            e_i = (e_{i-1} < d) ? e_{i-1} + d : e_{i-1} - d. Then a(n) = e_k
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1 */

    /* The maximum number of divisors for n <= 10000 is < 200; 2048 is safely large. */
    enum { CAP = 2048 };
    idx_t small[CAP];
    idx_t large[CAP];
    idx_t cs = 0, cl = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            small[cs++] = i;
            idx_t d2 = n / i;
            if (d2 != i) {
                large[cl++] = d2;
            }
        }
    }

    seq_t e = 0;

    /* process small divisors (already ascending) */
    for (idx_t i = 0; i < cs; ++i) {
        seq_t d = (seq_t)small[i];
        e = (e >= d) ? (e - d) : (e + d);
    }

    /* process large divisors in reverse to keep ascending order */
    for (idx_t j = cl; j-- > 0; ) {
        seq_t d = (seq_t)large[j];
        e = (e >= d) ? (e - d) : (e + d);
    }

    return e;
}
