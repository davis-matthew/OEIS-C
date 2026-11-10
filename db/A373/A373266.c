/*
 * A373266  a(n) = 1 if A276085(n) is of the form 8m+4, otherwise 0, where
 *          A276085 is the primorial base log-function.
 * https://oeis.org/A373266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A276085(n) == 4 (mod 8), else 0; A276085 is fully
 *            additive with a(p) = (product of primes < p), so the code sums
 *            e*prod(primes<p) mod 8 over p^e || n (sieve-based)
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 9707 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    uint64_t u = (uint64_t)n;
    uint64_t fp[64];
    uint64_t fe[64];
    int fc = 0;
    uint64_t r = u;
    for (uint64_t d = 2; d * d <= r; d += (d == 2 ? 1 : 2)) {
        if (r % d == 0) {
            uint64_t e = 0;
            while (r % d == 0) { r /= d; e++; }
            fp[fc] = d; fe[fc] = e; fc++;
        }
    }
    if (r > 1) { fp[fc] = r; fe[fc] = 1; fc++; }

    uint64_t maxp = 0;
    for (int i = 0; i < fc; i++) if (fp[i] > maxp) maxp = fp[i];

    unsigned char *comp = (unsigned char *)calloc((size_t)maxp + 1, 1);
    if (!comp) return (seq_t)0;
    for (uint64_t i = 2; i * i <= maxp; i++) {
        if (!comp[i]) {
            for (uint64_t j = i * i; j <= maxp; j += i) comp[j] = 1;
        }
    }
    uint64_t prod = 1;   /* product of primes < i, mod 8 */
    uint64_t total = 0;  /* A276085(n) mod 8 */
    for (uint64_t i = 2; i <= maxp; i++) {
        if (comp[i]) continue;
        for (int k = 0; k < fc; k++) {
            if (fp[k] == i) {
                total = (total + (fe[k] & 7) * prod) & 7;
                break;
            }
        }
        prod = (prod * (i & 7)) & 7;
    }
    free(comp);
    return (seq_t)(total == 4 ? 1 : 0);
}
