/*
 * A354526  Numbers k such that A354512(k) < omega(k); complement of A354525.
 * https://oeis.org/A354526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 158, u16 n <= 7946, u32 n <= 7946, u64 n <= 7946,
 *            u128 n <= 7946, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by on‑demand search: for each integer k>1 examine its
 * distinct prime factors p; if gpf(k+p)≠p for any such p, then k is a
 * term of the sequence.  The n‑th term is returned.  No external library
 * functions are relied upon, so the code is portable to both C11 and C++17
 * without any special treatment of arbitrary‑precision seq_t types. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n==0; harmless */

    idx_t count = 0;   /* how many terms found so far */
    uint64_t k = 2;    /* start checking at 2 */

    for (;;) {
        uint64_t m = k;
        bool good = false;
        uint64_t last_p = 0;

        /* factor k, considering each distinct prime factor once */
        while (m > 1) {
            uint64_t p;

            /* smallest prime factor of m */
            if (m % 2 == 0) {
                p = 2;
            } else {
                p = 0;
                for (uint64_t d = 3; d * d <= m; d += 2) {
                    if (m % d == 0) { p = d; break; }
                }
                if (p == 0) p = m;   /* m itself is prime */
            }

            if (p != last_p) {
                uint64_t kp = k + p;

                /* greatest prime factor of kp */
                uint64_t t = kp, g = 1;
                if (t % 2 == 0) { g = 2; while (t % 2 == 0) t /= 2; }
                for (uint64_t d = 3; d * d <= t; d += 2) {
                    if (t % d == 0) { g = d; while (t % d == 0) t /= d; }
                }
                if (t > 1) g = t;

                if (g != p) { good = true; break; }
            }

            /* remove all occurrences of p from m */
            while (m % p == 0) m /= p;
            last_p = p;
        }

        if (good) {
            ++count;
            if (count == n) return (seq_t)k;
        }

        ++k;
    }
}
