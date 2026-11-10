/*
 * A279519  a(n) = A049502(A003418(n)).
 * https://oeis.org/A279519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A049502(A003418(n)) = sum of (p+1) over the bit positions
 *            p * (0-based) where binary lcm(1..n) has bit p = 1 and bit p+1 =
 *            0, with the * top bit left out. lcm(1..n) is the product of
 *            p^floor(log_p(n)) over primes * p <= n. It is built as a
 *            multi-limb integer so it cannot overflow seq_t
 * Range:     u8 n <= 30, u16 n <= 478, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9794 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* bits of lcm(1..n) ~ 1.4427*n; allocate generously */
    size_t cap = (size_t)((n * 3) / 2 / 32 + 4);
    uint32_t *L = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t len = 1;
    L[0] = 1;

    /* sieve primes up to n */
    unsigned char *comp = (unsigned char *)calloc((size_t)n + 2, 1);
    for (idx_t p = 2; p <= n; p++) {
        if (comp[p]) continue;
        for (idx_t q = p * p; q <= n; q += p) comp[q] = 1;
        /* largest power of p <= n */
        idx_t pk = p;
        while (pk <= n / p) pk *= p;
        /* L *= pk */
        uint64_t carry = 0;
        for (size_t i = 0; i < len; i++) {
            uint64_t t = (uint64_t)L[i] * (uint64_t)pk + carry;
            L[i] = (uint32_t)t;
            carry = t >> 32;
        }
        while (carry) {
            if (len == cap) {
                cap *= 2;
                L = (uint32_t *)realloc(L, cap * sizeof(uint32_t));
            }
            L[len++] = (uint32_t)carry;
            carry >>= 32;
        }
    }
    free(comp);

    /* total bit length */
    while (len > 1 && L[len - 1] == 0) len--;
    uint32_t top = L[len - 1];
    idx_t topbits = 0;
    while (top) { topbits++; top >>= 1; }
    idx_t nbits = (idx_t)(len - 1) * 32 + topbits;

    seq_t sum = 0;
    if (nbits >= 2) {
        for (idx_t p = 0; p + 1 < nbits; p++) {
            int b0 = (L[p >> 5] >> (p & 31)) & 1;
            idx_t q = p + 1;
            int b1 = (L[q >> 5] >> (q & 31)) & 1;
            if (b0 == 1 && b1 == 0) sum += (seq_t)(p + 1);
        }
    }
    free(L);
    return sum;
}
