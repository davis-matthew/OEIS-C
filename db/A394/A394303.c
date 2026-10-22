/*
 * A394303  a(n) is the number of unit squares in the square lattice, divided
 *          by 4, that have 3 vertices strictly inside and 1 vertex strictly
 *          outside the circle (x-1/2)^2 + (y-1/2)^2 = n, i.e., the circle
 *          centered at the deep hole.
 * https://oeis.org/A394303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (number of unit squares of the integer lattice having
 *            three vertices strictly inside the circle (x‑1/2)² + (y‑1/2)² =
 *            n and one vertex strictly outside) / 4
 * Range:     u8 n <= 16, u16 n <= 2377, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of an unsigned 64‑bit integer.
   Uses only integer arithmetic. */
static inline idx_t _isqrt_u64(idx_t v)
{
    idx_t r = 0, bit = (idx_t)1 << 62;           /* 2^62 ≤ 2^63 <= max(idx_t) */
    while (bit) {
        idx_t t = r | bit;
        if ((__uint128_t)t * t <= v) r = t;
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Radius squared scaled by 4: (2√n)² = 4n. */
    idx_t rad_u64 = (idx_t)n * 4;
    seq_t rad4 = (seq_t)n * 4;

    /* Calculate an upper bound on |x-0.5| and |y-0.5| that can
       possibly touch the circle.  The bound is floor(2√n) + 3,
       which is safely large. */
    idx_t bound = _isqrt_u64(rad_u64) + 3;

    /* Convert to signed type for loop limits.
       The ranges are small (for n ≤ 10000, |x| ≤ ~210). */
    long long B = (long long)bound;          /* signed safety bound */
    long long u_min = -(2 * B - 1);          /* smallest odd u = 2x-1      */
    long long u_max =  (2 * B - 1);
    long long v_min = u_min;
    long long v_max = u_max;

    seq_t count = 0;

    for (long long u = u_min; u <= u_max; u += 2) {   /* u = 2x-1 (odd) */
        for (long long v = v_min; v <= v_max; v += 2) { /* v = 2y-1 (odd) */

            /* Squared distances to the origin (scaled by 4). */
            seq_t d00 = (seq_t)u * (seq_t)u + (seq_t)v * (seq_t)v;
            if (d00 == rad4) continue;                 /* on the circle */

            seq_t d10 = (seq_t)(u + 2) * (seq_t)(u + 2) + (seq_t)v * (seq_t)v;
            if (d10 == rad4) continue;

            seq_t d01 = (seq_t)u * (seq_t)u + (seq_t)(v + 2) * (seq_t)(v + 2);
            if (d01 == rad4) continue;

            seq_t d11 = (seq_t)(u + 2) * (seq_t)(u + 2) +
                        (seq_t)(v + 2) * (seq_t)(v + 2);
            if (d11 == rad4) continue;

            int outside = 0;
            if (d00 > rad4) ++outside;
            if (d10 > rad4) ++outside;
            if (d01 > rad4) ++outside;
            if (d11 > rad4) ++outside;

            if (outside == 1) ++count;
        }
    }

    return count / 4;   /* divide by 4 as per the sequence definition */
}
