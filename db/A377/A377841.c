/*
 * A377841  Index of first occurrence of n in A375277, or -1 if n does not
 *          appear there.
 * https://oeis.org/A377841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = least k >= 1 with k! mod nextprime(k) = n (A375277), or
 *            -1 if none is found; computed by a search using Wilson's
 *            theorem, (p-1)! == -1 (mod p), over blocks of k between
 *            consecutive primes (search limit 2^25)
 * Range:     u8 n <= 12, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define A377841_LIMIT 33554432u

static uint8_t *A377841_comp = 0;

static void A377841_init(void) {
    if (A377841_comp) return;
    uint32_t L = A377841_LIMIT;
    A377841_comp = (uint8_t *)calloc((size_t)(L / 2 + 2), 1); /* index i -> 2i+1 */
    for (uint64_t i = 3; i * i <= L; i += 2)
        if (!A377841_comp[i / 2])
            for (uint64_t j = i * i; j <= L; j += 2 * i)
                A377841_comp[j / 2] = 1;
}

static uint64_t A377841_inv(uint64_t a, uint64_t m) {
    int64_t t = 0, nt = 1, r = (int64_t)m, nr = (int64_t)(a % m);
    while (nr) {
        int64_t q = r / nr, tmp;
        tmp = t - q * nt; t = nt; nt = tmp;
        tmp = r - q * nr; r = nr; nr = tmp;
    }
    if (t < 0) t += (int64_t)m;
    return (uint64_t)t;
}

seq_t a(idx_t n) {
    A377841_init();
    uint64_t v = (uint64_t)n;
    uint64_t prev = 1;
    for (uint64_t p = 2; p <= A377841_LIMIT; p++) {
        bool pr = (p == 2) || ((p & 1) && !A377841_comp[p / 2]);
        if (!pr) continue;
        if (v < p) {
            uint64_t prod = 1;
            uint64_t best = 0;
            for (uint64_t k = p - 1; k + 1 > prev && k >= 1; k--) {
                uint64_t val = ((p - 1) * A377841_inv(prod, p)) % p;
                if (val == v) best = k;
                prod = (prod * (k % p)) % p;
                if (k == prev) break;
            }
            if (best) return (seq_t)best;
        }
        prev = p;
    }
    return (seq_t)0 - 1;
}
