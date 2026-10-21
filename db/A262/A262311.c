/*
 * A262311  Number of ordered ways to write n = x^2 + y^2 + phi(z^2) (0 <= x
 *          <= y and z > 0) with y or z prime, where phi(.) is Euler's totient
 *          function given by A000010.
 * https://oeis.org/A262311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (x,y,z) with 0 <= x <= y, z >
 *            0, n = x^2 + y^2 + φ(z^2), and with y or z prime
 * Range:     u8 n <= 16, u16 n <= 2360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_phi_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author: Adapted for arbitrary-precision seq_t types. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sieve_phi_prime.h"

/* Integer floor square root for 64‑bit values. */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * (__int128)mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Required public function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    /* Allocate temporary arrays for φ and prime. */
    idx_t *phi = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    bool   *prime = (bool*)malloc((n + 1) * sizeof(bool));
    if (!phi || !prime) {
        free(phi); free(prime);
        return (seq_t)0;                /* on OOM we return 0 */
    }

    /* Build φ and prime tables up to n. */
    sieve_phi_prime(n, phi, prime);

    seq_t result = 0;

    /* Enumerate all admissible triples. */
    for (idx_t z = 1; z <= n; ++z) {
        seq_t phi_z2 = (seq_t)z * (seq_t)phi[z];
        if (phi_z2 > n) continue;              /* φ(z²) too large */

        bool z_prime = prime[z];
        idx_t rem_phi = (idx_t)n - (idx_t)phi_z2;   /* remaining for x²+y² */

        idx_t max_y = isqrt_u64(rem_phi);
        for (idx_t y = 0; y <= max_y; ++y) {
            if (!z_prime && !prime[y]) continue;   /* neither prime */

            idx_t y2 = y * y;
            if (y2 > rem_phi) break;               /* safety */

            idx_t rem_xy = rem_phi - y2;
            idx_t max_x = isqrt_u64(rem_xy);
            if (max_x > y) max_x = y;              /* enforce x <= y */

            for (idx_t x = 0; x <= max_x; ++x) {
                seq_t val = (seq_t)((idx_t)x * (idx_t)x + y2 + (idx_t)phi_z2);
                if (val == n) result += 1;
            }
        }
    }

    free(phi);
    free(prime);
    return result;
}
