/*
 * A375785  a(n) is the number of distinct integer-sided cuboids having the
 *          same surface as a cube with edge length n.
 * https://oeis.org/A375785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered positive integer solutions (x,y,z) to
 *            x*y + y*z + x*z = 3*n*n
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root floor for unsigned 64‑bit quantities. */
static inline idx_t isqrt_u64(idx_t v)
{
    if (v <= 1) return v;
    idx_t bit = 1ULL << (63 - __builtin_clzll(v)); /* highest power of two ≤ v */
    idx_t res = 0;
    while (bit) {
        idx_t temp = res + bit;
        if (v / temp >= temp) {
            res = temp;
        }
        bit >>= 1;
    }
    return res;
}

/* Number of distinct integer‑sided cuboids having the same surface as
   a cube with edge length n. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* safety for offset 0 */

    idx_t S = 3 * n * n;                     /* target value 3*n^2 */
    uint64_t count = 0;                      /* use 64‑bit counter */

    /* Upper bound for x: x*y ≤ S ⇒ x ≤ √S. */
    idx_t xmax = isqrt_u64(S);

    for (idx_t x = 1; x <= xmax; ++x) {
        /* For a fixed x, y must satisfy (x + y)² ≤ x² + S.
           Thus y ≤ √(x² + S) – x. */
        idx_t tmp = x * x + S;
        idx_t sqrt_tmp = isqrt_u64(tmp);
        if (sqrt_tmp <= x) continue;          /* no admissible y for this x */
        idx_t ymax = sqrt_tmp - x;

        for (idx_t y = x; y <= ymax; ++y) {
            idx_t xy = x * y;
            if (xy > S) break;                /* numerator would be negative */

            idx_t denom = x + y;
            idx_t numer = S - xy;
            if (numer % denom != 0) continue; /* z not integer */

            idx_t z = numer / denom;
            if (z >= y) ++count;              /* enforce x ≤ y ≤ z */
        }
    }

    return (seq_t)count;
}
