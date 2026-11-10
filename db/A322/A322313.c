/*
 * A322313  Lexicographically earliest such sequence a that a(i) = a(j) =>
 *          A322312(i) = A322312(j) for all i, j.
 * https://oeis.org/A322313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of A322312, where A322312(n)
 *            = Product_{p prime, (p-1)|n} prime(1+v_p(n)); equal values <=>
 *            equal multiset of exponents v_p(n), compared directly
 * Range:     u8 n <= 4355, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9863 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define A322313_SL 64

static bool A322313_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

static void A322313_add(uint64_t m, uint64_t d, uint16_t *sig)
{
    uint64_t p = d + 1;
    if (!A322313_isprime(p)) return;
    unsigned e = 0;
    uint64_t t = m;
    while (t % p == 0) { t /= p; e++; }
    if (e >= A322313_SL) e = A322313_SL - 1;
    sig[e]++;
}

seq_t a(idx_t n)
{
    static uint32_t *val = NULL;     /* val[m] for m = 1..cnt */
    static size_t cnt = 0;
    static size_t cap = 0;
    static uint16_t *cls = NULL;     /* class signatures */
    static size_t ncls = 0;
    static size_t clscap = 0;

    if (n < 1) return 0;
    if ((size_t)n > cap) {
        size_t nc = cap ? cap : 64;
        while (nc < (size_t)n) nc *= 2;
        val = (uint32_t *)realloc(val, (nc + 1) * sizeof(uint32_t));
        if (!val) abort();
        cap = nc;
    }
    while (cnt < (size_t)n) {
        uint64_t m = (uint64_t)cnt + 1;
        uint16_t sig[A322313_SL];
        memset(sig, 0, sizeof(sig));
        for (uint64_t i = 1; i * i <= m; i++) {
            if (m % i == 0) {
                A322313_add(m, i, sig);
                if (i != m / i) A322313_add(m, m / i, sig);
            }
        }
        size_t found = 0;
        for (size_t c = 0; c < ncls; c++) {
            if (memcmp(cls + c * A322313_SL, sig, sizeof(sig)) == 0) {
                found = c + 1;
                break;
            }
        }
        if (!found) {
            if (ncls == clscap) {
                clscap = clscap ? clscap * 2 : 64;
                cls = (uint16_t *)realloc(cls, clscap * A322313_SL * sizeof(uint16_t));
                if (!cls) abort();
            }
            memcpy(cls + ncls * A322313_SL, sig, sizeof(sig));
            ncls++;
            found = ncls;
        }
        cnt++;
        val[cnt] = (uint32_t)found;
    }
    return (seq_t)val[(size_t)n];
}
