/*
 * A369456  a(n) = A083345(A005940(1+n)), where A083345(n) = (n'/gcd(n,n')),
 *          n' means the arithmetic derivative of n (A003415), and A005940 is
 *          the Doudna-sequence.
 * https://oeis.org/A369456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A083345(A005940(1+n)) = numerator of Sum e_k/prime(k),
 *            where the exponents e_k come from the binary expansion of n
 *            (Doudna sequence). Reduced numerator computed as Sum_k e_k * R /
 *            prime(k), where R = product of prime(k) with prime(k) not
 *            dividing e_k (all terms integral, no overflow)
 * Range:     u8 n <= 148, u16 n <= 2643, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 104 terms through n = 7991 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A369456_primes[72];
static int A369456_ready = 0;

static void A369456_init(void) {
    int count = 0;
    uint64_t c = 1;
    while (count < 72) {
        c++;
        bool pr = true;
        for (uint64_t d = 2; d * d <= c; d++) {
            if (c % d == 0) { pr = false; break; }
        }
        if (pr) A369456_primes[count++] = c;
    }
    A369456_ready = 1;
}

seq_t a(idx_t n) {
    if (!A369456_ready) A369456_init();
    int ex[72];
    for (int k = 0; k < 72; k++) ex[k] = 0;
    idx_t m = n;
    int i = 0;
    while (m > 0) {
        if (m & 1) {
            ex[i]++;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }
    /* nondiv[k]: prime(k) present and does not divide its exponent */
    bool nondiv[72];
    for (int k = 0; k < 72; k++)
        nondiv[k] = ex[k] > 0 && ((uint64_t)ex[k] % A369456_primes[k]) != 0;

    seq_t result = 0;
    for (int k = 0; k < 72; k++) {
        if (!ex[k]) continue;
        uint64_t p = A369456_primes[k];
        if (nondiv[k]) {
            /* e_k * (R / p_k) */
            seq_t t = (seq_t)ex[k];
            for (int j = 0; j < 72; j++)
                if (j != k && nondiv[j]) t *= (seq_t)A369456_primes[j];
            result += t;
        } else {
            /* (e_k / p_k) * R */
            seq_t t = (seq_t)((uint64_t)ex[k] / p);
            for (int j = 0; j < 72; j++)
                if (nondiv[j]) t *= (seq_t)A369456_primes[j];
            result += t;
        }
    }
    return result;
}
