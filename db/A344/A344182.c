/*
 * A344182  a(n) = A344026(n) XOR A344028(n).
 * https://oeis.org/A344182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(A005940(1+n)) XOR A069359(A005940(1+n)). The
 *            prime exponents of m = A005940(n+1) are read from the binary
 *            expansion of n, then A003415(m) = Sum_{p|m} e_p*(m/p) and
 *            A069359(m) = Sum_{p|m} m/p, with each m/p formed directly as a
 *            product
 * Range:     u8 n <= 56, u16 n <= 495, u32 n <= 16239, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 100 terms through n = 9354 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A344182_primes[70];
static int A344182_have_primes = 0;

static void A344182_init_primes(void) {
    if (A344182_have_primes) return;
    int c = 0;
    for (uint64_t q = 2; c < 70; q++) {
        bool ok = true;
        for (uint64_t d = 2; d * d <= q; d++) {
            if (q % d == 0) { ok = false; break; }
        }
        if (ok) A344182_primes[c++] = q;
    }
    A344182_have_primes = 1;
}

seq_t a(idx_t n) {
    int e[70];
    int i, j, l, cnt;
    idx_t k = n;
    A344182_init_primes();
    for (i = 0; i < 70; i++) e[i] = 0;
    i = 0;
    while (k > 0) {
        if (k & 1) e[i]++;
        else i++;
        k >>= 1;
    }
    cnt = i + 1;
    if (cnt > 70) cnt = 70;

    seq_t deriv = 0, sumdiv = 0;
    for (j = 0; j < cnt; j++) {
        if (e[j] == 0) continue;
        seq_t t = 1;  /* m / p_j */
        for (l = 0; l < cnt; l++) {
            int ex = e[l] - (l == j ? 1 : 0);
            for (int r = 0; r < ex; r++) t *= (seq_t)A344182_primes[l];
        }
        deriv += t * (seq_t)e[j];
        sumdiv += t;
    }
    return deriv ^ sumdiv;
}
