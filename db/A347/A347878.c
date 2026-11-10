/*
 * A347878  Numbers k for which A003415(sigma(k)) is even.
 * https://oeis.org/A347878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A003415(sigma(k)) is even; found by
 *            search. Parity of m' : m even -> odd iff v2(m)=1; m odd ->
 *            parity of bigomega(m)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A347878_sigma(uint64_t k) {
    uint64_t res = 1;
    for (uint64_t p = 2; p * p <= k; p++) {
        if (k % p == 0) {
            uint64_t term = 1, pw = 1;
            while (k % p == 0) {
                k /= p;
                pw *= p;
                term += pw;
            }
            res *= term;
        }
    }
    if (k > 1) res *= (k + 1);
    return res;
}

static bool A347878_deriv_even(uint64_t m) {
    if (m <= 1) return true;
    if ((m & 1ULL) == 0) {
        int e = 0;
        while ((m & 1ULL) == 0) { m >>= 1; e++; }
        return e != 1;
    }
    int cnt = 0;
    for (uint64_t p = 3; p * p <= m; p += 2) {
        while (m % p == 0) { m /= p; cnt++; }
    }
    if (m > 1) cnt++;
    return (cnt & 1) == 0;
}

seq_t a(idx_t n) {
    idx_t found = 0;
    uint64_t k = 0;
    while (found < n) {
        k++;
        if (A347878_deriv_even(A347878_sigma(k))) found++;
    }
    return (seq_t)k;
}
