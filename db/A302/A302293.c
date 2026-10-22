/*
 * A302293  Number of positive integer pairs (x,y) such that there exist
 *          positive integers p and q satisfying p*x^2 + q*y = n.
 * https://oeis.org/A302293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of positive integer pairs (x,y) for which there exist positive
   integers p,q with p*x^2 + q*y = n.  The implementation uses
   a brute‑force search with a O(n^1.5) bound (about 10^6 operations for
   n≈10000).  */
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Modular inverse for 64‑bit integers (extended Euclid).  m > 0. */
static idx_t modinv_idx(idx_t a, idx_t m) {
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)m, newr = (int64_t)a;
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = t - q * newt;
        t = newt;
        newt = tmp;
        tmp = r - q * newr;
        r = newr;
        newr = tmp;
    }
    if (t < 0) t += m;
    return (idx_t)t;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    if (n <= 1) return 0;

    idx_t max_x = 0;
    /* largest x with x^2 <= n-1 */
    while ((max_x + 1) * (max_x + 1) <= n - 1) ++max_x;

    for (idx_t x = 1; x <= max_x; ++x) {
        idx_t x2 = x * x;
        idx_t limit_p = (n - 1) / x2;          /* maximum p allowed */
        for (idx_t y = 1; y <= n - 1; ++y) {
            seq_t g_seq = gcd((seq_t)x2, (seq_t)y);
            idx_t g = (idx_t)g_seq;
            if (n % g != 0) continue;          /* congruence impossible */

            idx_t m = y / g;                    /* modulus for p */
            idx_t x2p = x2 / g;
            idx_t np = n / g;

            idx_t inv = 0;
            if (m > 1) {
                inv = modinv_idx(x2p % m, m);
            }
            idx_t p0 = (inv * (np % m)) % m;
            if (p0 == 0) p0 = m;                /* smallest positive p */

            if (p0 <= limit_p) {
                ++count;
            }
        }
    }
    return count;
}
