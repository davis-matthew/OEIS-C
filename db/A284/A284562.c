/*
 * A284562  Product / LCM of the lengths of 1-runs in binary representation of
 *          n: a(n) = A227349(n) / A284569(n).
 * https://oeis.org/A284562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A227349(n) / A284569(n) (product of run lengths / lcm of
 *            run lengths)
 * Range:     u8 n <= 9503, u16 n <= 9503, u32 n <= 9503, u64 n <= 9503,
 *            u128 n <= 9503, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, if needed later */

static seq_t _gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Handle n==0 explicitly (empty product and lcm both 1) */
    if (n == (idx_t)0)
        return (seq_t)1;

    idx_t x = n;
    seq_t product = (seq_t)1;     /* product of lengths of 1-runs */
    seq_t lcm_val = (seq_t)1;     /* lcm of lengths of 1-runs */
    unsigned int run = 0;         /* current run of consecutive 1s */

    while (x > (idx_t)0) {
        if (x & (idx_t)1) {
            /* still in a run of 1s */
            run++;
        } else {
            /* run ended, update product and lcm if run was non‑empty */
            if (run > 0) {
                product *= (seq_t)run;
                seq_t g = _gcd_seq_t(lcm_val, (seq_t)run);
                lcm_val = (lcm_val / g) * (seq_t)run;
                run = 0;
            }
        }
        x >>= (idx_t)1;
    }

    /* handle a trailing run at the end of the binary representation */
    if (run > 0) {
        product *= (seq_t)run;
        seq_t g = _gcd_seq_t(lcm_val, (seq_t)run);
        lcm_val = (lcm_val / g) * (seq_t)run;
    }

    /* product divided by lcm is always an integer */
    return product / lcm_val;
}
