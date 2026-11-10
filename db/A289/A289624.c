/*
 * A289624  a(n) = A002322(n)/A007947(A034380(n)).
 * https://oeis.org/A289624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002322(n) / A007947(A034380(n)), where A034380(n) =
 *            phi(n)/lambda(n), lambda = Carmichael function, A007947 =
 *            squarefree kernel
 * Range:     u8 n <= 256, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 110 terms through n = 8089 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t a289624_gcd(uint64_t x, uint64_t y) {
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static uint64_t a289624_rad(uint64_t m) {
    uint64_t r = 1;
    for (uint64_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            r *= p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) r *= m;
    return r;
}

seq_t a(idx_t n_idx) {
    uint64_t n = (uint64_t)n_idx;
    uint64_t phi = 1, lam = 1;
    for (uint64_t p = 2; p <= n / p; p++) {
        if (n % p == 0) {
            uint64_t e = 0, pe1 = 1;
            while (n % p == 0) {
                n /= p;
                e++;
                if (e > 1) pe1 *= p;
            }
            /* pe1 = p^(e-1) */
            uint64_t ph = pe1 * (p - 1);
            uint64_t lp;
            if (p == 2) {
                if (e == 1) lp = 1;
                else if (e == 2) lp = 2;
                else lp = pe1 / 2;
            } else {
                lp = ph;
            }
            phi *= ph;
            lam = lam / a289624_gcd(lam, lp) * lp;
        }
    }
    if (n > 1) {
        phi *= (n - 1);
        lam = lam / a289624_gcd(lam, n - 1) * (n - 1);
    }
    uint64_t ratio = phi / lam;
    return (seq_t)(lam / a289624_rad(ratio));
}
