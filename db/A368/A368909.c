/*
 * A368909  a(n) = A003415(sigma(A005940(1+n))) mod 2, where A003415 is the
 *          arithmetic derivative, sigma is the sum of divisors function, and
 *          A005940 is the Doudna sequence.
 * https://oeis.org/A368909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(sigma(A005940(n+1))) mod 2; the parity of
 *            A003415(S) is computed from the factorization of S =
 *            sigma(A005940(n+1))
 * Range:     u8 n <= 53, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 138 terms through n = 8362 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool dd_isp(unsigned x) {
    if (x < 2) return false;
    for (unsigned d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n) {
    unsigned ex[72];
    for (int k = 0; k < 72; k++) ex[k] = 0;
    idx_t m = n;          /* A005940(n+1) uses binary of n */
    int i = 0;
    while (m > 0) {
        if (m & 1) ex[i]++;
        else i++;
        m >>= 1;
    }
    /* Note: odd step keeps i; even step increments i (after a 1-bit, shift). */
    /* Recompute properly following f(n,i,x) */
    for (int k = 0; k < 72; k++) ex[k] = 0;
    m = n;
    i = 0;
    while (m > 0) {
        if (m & 1) { ex[i]++; m = (m - 1) / 2; }
        else { m /= 2; i++; }
    }
    /* sigma of the Doudna number */
    seq_t S = 1;
    unsigned p = 1;
    for (int k = 0; k <= i && k < 72; k++) {
        do { p++; } while (!dd_isp(p));
        if (ex[k] == 0) continue;
        seq_t term = 1, pw = 1;
        for (unsigned j = 0; j < ex[k]; j++) {
            pw *= (seq_t)p;
            term += pw;
        }
        S *= term;
    }
    /* parity of arithmetic derivative of S */
    if (S <= 1) return 0;
    int e0 = 0;
    while ((S & 1) == 0) { S >>= 1; e0++; }
    if (e0 == 1) return 1;
    if (e0 >= 2) return 0;
    unsigned par = 0;
    for (seq_t q = 3; q * q <= S; q += 2) {
        if (S % q == 0) {
            while (S % q == 0) { S /= q; par ^= 1; }
        }
    }
    if (S > 1) par ^= 1;
    return (seq_t)par;
}
