/*
 * A356137  Positive integers m such that the fractional part of the geometric
 *          mean of the sequence s(m) does not exceed the fractional part of
 *          the arithmetic mean of s(m), where s(m) is the sequence 1 + 1/1, 2
 *          + 1/2, ..., m + 1/m.
 * https://oeis.org/A356137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer m such that the fractional part * of
 *            the geometric mean of the sequence s(m) = (1+1/1, 2+1/2, …,
 *            m+1/m) * does not exceed the fractional part of the arithmetic
 *            mean of s(m)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n == 0, but the offset starts at 1 */

    /* static state for incremental search and caching */
    static idx_t last_checked = 0;          /* largest m examined so far   */
    static long double sum_harm = 0.0L;      /* Σ 1/i for i = 1 .. m         */
    static long double sum_log = 0.0L;       /* Σ ln(i + 1/i) for i = 1 .. m */

    static seq_t *values = NULL;            /* cached terms of the sequence */
    static size_t values_len = 0, values_cap = 0;

    /* ensure storage capacity for new terms */
    if (values_len == values_cap) {
        values_cap = values_cap ? values_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(values, values_cap * sizeof(seq_t));
        if (!tmp)
            return 0;  /* allocation failure – abort */
        values = tmp;
    }

    /* extend search until we have at least n terms cached */
    while (values_len < n) {
        idx_t m = ++last_checked;

        /* update cumulative sums for new m */
        sum_harm += 1.0L / (long double)m;
        sum_log += logl((long double)m + 1.0L / (long double)m);

        /* arithmetic mean of s(m) */
        long double AM = ((long double)m + 1.0L) / 2.0L + sum_harm / (long double)m;
        long double fracA = AM - floorl(AM);

        /* geometric mean of s(m) */
        long double GM = expl(sum_log / (long double)m);
        long double fracG = GM - floorl(GM);

        /* compare fractional parts with a tiny tolerance */
        const long double eps = 1e-15L;
        if (fracG <= fracA + eps) {
            /* store m as a valid term */
            if (values_len == values_cap) {
                values_cap = values_cap ? values_cap * 2 : 16;
                seq_t *tmp2 = (seq_t *)realloc(values, values_cap * sizeof(seq_t));
                if (!tmp2)
                    return 0;  /* allocation failure – abort */
                values = tmp2;
            }
            values[values_len++] = (seq_t)m;
        }
    }

    return values[n - 1];
}
