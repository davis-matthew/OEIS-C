/*
 * A278445  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 43", based on the 5-celled von
 *          Neumann neighborhood.
 * https://oeis.org/A278445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = a(n-1) + 4*a(n-2)
 *            - 4*a(n-3) + 4*a(n-4) - 4*a(n-5) - 16*a(n-6) + 16*a(n-7), * for
 *            n ≥ 8. * Initial values: * a(0)=1, a(1)=1, a(2)=1, a(3)=13,
 *            a(4)=5, * a(5)=53, a(6)=5, a(7)=245. * The recurrence is derived
 *            from the rational generating function * (1-4...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return the first eight terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)13;
    if (n == 4) return (seq_t)5;
    if (n == 5) return (seq_t)53;
    if (n == 6) return (seq_t)5;
    if (n == 7) return (seq_t)245;

    /* Circular buffer holding the last eight terms: a(i‑7)…a(i‑1). */
    seq_t buf[8] = {
        (seq_t)1,  /* a(0) */
        (seq_t)1,  /* a(1) */
        (seq_t)1,  /* a(2) */
        (seq_t)13, /* a(3) */
        (seq_t)5,  /* a(4) */
        (seq_t)53, /* a(5) */
        (seq_t)5,  /* a(6) */
        (seq_t)245 /* a(7) */
    };

    for (idx_t i = 8; i <= n; ++i) {
        idx_t im1 = (i - 1) % 8;   /* a(i-1)      */
        idx_t im2 = (i - 2) % 8;   /* a(i-2)      */
        idx_t im3 = (i - 3) % 8;   /* a(i-3)      */
        idx_t im4 = (i - 4) % 8;   /* a(i-4)      */
        idx_t im5 = (i - 5) % 8;   /* a(i-5)      */
        idx_t im6 = (i - 6) % 8;   /* a(i-6)      */
        idx_t im7 = (i - 7) % 8;   /* a(i-7)      */

        /* Positive part: a(i-1) + 4*a(i-2) + 4*a(i-4) + 16*a(i-7) */
        seq_t pos = buf[im1];
        pos = pos + (buf[im2] << 2);  /* 4 * a(i-2) */
        pos = pos + (buf[im4] << 2);  /* 4 * a(i-4) */
        pos = pos + (buf[im7] << 4);  /* 16* a(i-7) */

        /* Negative part: 4*a(i-3) + 4*a(i-5) + 16*a(i-6) */
        seq_t neg = (buf[im3] << 2);  /* 4 * a(i-3) */
        neg = neg + (buf[im5] << 2);  /* 4 * a(i-5) */
        neg = neg + (buf[im6] << 4);  /* 16* a(i-6) */

        /* New term */
        seq_t next = pos - neg;

        /* Store the new term in the circular buffer. */
        buf[i % 8] = next;
    }

    return buf[n % 8];
}
