/*
 * A350678  Partial sums of A185381.
 * https://oeis.org/A350678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A185381(k) A185381(k) = Fibonacci( floor(k *
 *            φ) ), φ = (1 + √5) / 2
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* golden ratio */
    const long double phi = (1.0L + sqrtl(5.0L)) / 2.0L;

    seq_t sum = 0;
    idx_t k = 1;
    idx_t m = (idx_t)floorl((long double)k * phi);   /* floor(k*φ) */
    seq_t f_prev = 0;   /* F_{m-1} */
    seq_t f_curr = 1;   /* F_{m}   */
    sum += f_curr;

    for (k = 2; k <= n; ++k) {
        idx_t m_next = (idx_t)floorl((long double)k * phi);
        idx_t inc = m_next - m;            /* should be 1 or 2 */

        if (inc == 1) {
            seq_t tmp = f_prev + f_curr;   /* F_{m+1} */
            f_prev = f_curr;
            f_curr = tmp;
        } else if (inc == 2) {
            seq_t f_next = f_prev + f_curr;       /* F_{m+1} */
            seq_t f_next_next = f_next + f_curr;  /* F_{m+2} */
            f_prev = f_next;
            f_curr = f_next_next;
        } else { /* fall‑back for safety, should never occur */
            for (idx_t j = 0; j < inc; ++j) {
                seq_t tmp = f_prev + f_curr;
                f_prev = f_curr;
                f_curr = tmp;
            }
        }
        m = m_next;
        sum += f_curr;
    }
    return sum;
}
