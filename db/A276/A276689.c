/*
 * A276689  Least term in the periodic part of the continued fraction
 *          expansion of sqrt(n) or 0 if n is square.
 * https://oeis.org/A276689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 0 if n is a perfect square * a(n) = least term in the
 *            periodic part of * the continued‑fraction expansion of √n
 *            otherwise
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer floor square root for 64‑bit unsigned values.
   Returns floor(sqrt(n)). */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = (idx_t)1ULL << 62;          /* highest usable bit (>>2 of max n) */
    while (bit) {
        idx_t next = res + bit;
        if ((__int128)next * next <= n) {
            res = next;
        }
        bit >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Perfect‑square check – return 0 if n is a square. */
    idx_t s = floor_sqrt_u64(n);
    if ((seq_t)s * (seq_t)s == (seq_t)n) {
        return (seq_t)0;
    }

    /* Continued fraction parameters. */
    idx_t  a0      = s;                 /* floor(√n) */
    seq_t  m       = 0;
    seq_t  d       = 1;
    seq_t  a_prev  = (seq_t)a0;
    seq_t  min_term = 0;                /* will be set on first iteration */
    bool   first   = true;              /* flag for first period term */

    for (;;) {
        /* Standard recurrence for the continued fraction of √n. */
        m = d * a_prev - m;
        d = ((seq_t)n - m * m) / d;
        seq_t a_k = ((seq_t)a0 + m) / d;   /* next term in period */

        if (first) {
            min_term = a_k;
            first = false;
        } else if (a_k < min_term) {
            min_term = a_k;
        }

        if (a_k == (seq_t)(2 * a0)) {
            break;          /* period finished */
        }
        a_prev = a_k;
    }

    return min_term;
}
