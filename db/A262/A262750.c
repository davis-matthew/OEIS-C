/*
 * A262750  Least positive integer z such that n - phi(z^2) = x^2 + y^2 for
 *          some integers x and y with x*y*z even and phi(k^2) < n for all 0 <
 *          k < z, or 0 if no such z exists, where phi(.) is Euler's totient
 *          function given by A000010.
 * https://oeis.org/A262750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer z such that * n - phi(z^2) = x^2 +
 *            y^2 for some integers x, y with x*y*z even, * phi(k^2) < n for
 *            all 0 < k < z; else 0
 * Range:     u8 n <= 31, u16 n <= 8164, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation searches z from 1 up to
 * floor(sqrt(2*n)) + 1, which is sufficient because
 * phi(z^2) >= z^2/2, so for z > sqrt(2*n) we always have
 * phi(z^2) > n and thus no further candidates can exist. */
 
#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t max_phi_prev = 0;                     /* max phi(k^2) for k < current z */
    /* search limit: floor(sqrt(2*n)) + 1   (using seq_t sqrt) */
    seq_t limit_seq = floor_sqrt((seq_t)n * 2) + 1;
    idx_t limit = (idx_t)limit_seq;

    for (idx_t z = 1; z <= limit; ++z) {
        /* compute phi(z^2) */
        seq_t phi_z2 = (__int128)z * z;          /* z^2 fits in 128-bit for our limits */
        idx_t m = z;
        for (idx_t p = 2; p * p <= m; ++p) {
            if (m % p == 0) {
                phi_z2 = phi_z2 / p * (p - 1);
                while (m % p == 0) m /= p;
            }
        }
        if (m > 1) {
            phi_z2 = phi_z2 / m * (m - 1);
        }

        /* candidate check: phi_z2 <= n and n > max_phi_prev */
        if (phi_z2 <= (seq_t)n && (seq_t)n > max_phi_prev) {
            seq_t d = (seq_t)n - phi_z2;
            if (d == 0) return (seq_t)z;                 /* 0 = 0^2 + 0^2, parity OK */

            bool ok = false;
            seq_t sqrt_d = floor_sqrt(d);

            /* search for a representation d = x^2 + y^2 */
            for (seq_t x = 0; x <= sqrt_d; ++x) {
                seq_t y2 = d - x * x;
                seq_t y = floor_sqrt(y2);
                if (y * y == y2) {
                    /* parity condition: at least one of x, y, z even */
                    if ((z % 2 == 0) || (x % 2 == 0) || (y % 2 == 0)) {
                        ok = true;
                        break;
                    }
                }
            }

            if (ok) return (seq_t)z;
        }

        /* update maximum phi up to current z */
        if (phi_z2 > max_phi_prev) max_phi_prev = phi_z2;
    }

    return (seq_t)0;          /* no such z found within the search limit */
}
