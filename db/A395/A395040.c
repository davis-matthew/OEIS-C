/*
 * A395040  a(n) = A276085(A003415(A276086(n))).
 * https://oeis.org/A395040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A276085(A003415(A276086(n))). A276086(n) = prod
 *            prime(k+1)^d_k (d_k = primorial-base digits of n); its
 *            derivative is N*Sum d_k/prime(k+1); that value is factored by
 *            trial division with a sieve up to 2e6, and A276085 is Sum
 *            e*(product of primes < p). Returns 0 if a prime factor exceeds
 *            2e6
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 28, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 241 (2026-11-10)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

#define A395040_LIMIT 2000000

static uint64_t *A395040_pl = NULL;
static size_t A395040_pc = 0;

static void A395040_init(void) {
    if (A395040_pl) return;
    build_primes_upto(A395040_LIMIT, &A395040_pl, &A395040_pc);
}

seq_t a(idx_t n) {
    static const unsigned sp[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    unsigned d[20];
    int k, nd = 0;
    uint64_t m = (uint64_t)n;
    while (m > 0 && nd < 20) {
        d[nd] = (unsigned)(m % sp[nd]);
        m /= sp[nd];
        nd++;
    }
    /* N = A276086(n) */
    seq_t N = 1;
    for (k = 0; k < nd; k++)
        for (unsigned j = 0; j < d[k]; j++)
            N *= (seq_t)sp[k];
    /* D = A003415(N) */
    seq_t D = 0;
    for (k = 0; k < nd; k++)
        if (d[k] > 0)
            D += (seq_t)d[k] * (N / (seq_t)sp[k]);
    if (D <= 1) return (seq_t)0;

    A395040_init();
    uint64_t fp[80];
    uint64_t fe[80];
    size_t fc = 0;
    seq_t rem = D;
    size_t i;
    bool exhausted = true;
    for (i = 0; i < A395040_pc; i++) {
        seq_t p = (seq_t)A395040_pl[i];
        if (p > rem / p) { exhausted = false; break; }
        if (rem % p == 0) {
            uint64_t e = 0;
            while (rem % p == 0) { rem /= p; e++; }
            if (fc < 80) { fp[fc] = A395040_pl[i]; fe[fc] = e; fc++; }
        }
    }
    (void)exhausted;
    if (rem > 1) {
        if (rem > (seq_t)A395040_LIMIT) return (seq_t)0;
        if (fc < 80) { fp[fc] = (uint64_t)rem; fe[fc] = 1; fc++; }
    }
    if (fc == 0) return (seq_t)0;

    seq_t result = 0;
    seq_t prod = 1;
    size_t f = 0;
    for (i = 0; i < A395040_pc && f < fc; i++) {
        uint64_t p = A395040_pl[i];
        if (p == fp[f]) {
            result += (seq_t)fe[f] * prod;
            f++;
        }
        prod *= (seq_t)p;
    }
    return result;
}
