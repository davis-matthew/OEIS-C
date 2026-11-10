/*
 * A243347  a(1)=1, and for n>1, if mu(n) = 0, a(n) =
 *          A005117(1+a(A057627(n))), otherwise, a(n) =
 *          A013929(a(A013928(n))).
 * https://oeis.org/A243347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n) = A005117(1+a(A057627(n))) if n is
 *            nonsquarefree, else a(n) = A013929(a(A013928(n))); computed by
 *            recursion with sieves
 * Range:     u8 n <= 16, u16 n <= 318, u32 n <= 520, u64 n <= 520,
 *            u128 n <= 520, big any n
 * Validated: standard, pass, 85 terms through n = 520 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* flags[i]=1 if i is squarefree, for 1<=i<=lim */
static uint8_t *sqf_sieve(uint64_t lim) {
    uint8_t *f = (uint8_t *)malloc((size_t)lim + 1);
    for (uint64_t i = 0; i <= lim; i++) f[i] = 1;
    f[0] = 0;
    for (uint64_t p = 2; p * p <= lim; p++) {
        uint64_t sq = p * p;
        for (uint64_t j = sq; j <= lim; j += sq) f[j] = 0;
    }
    return f;
}

/* k-th number with squarefree flag == want (want=1 squarefree, 0 not) */
static uint64_t nth_kind(uint64_t k, int want) {
    uint64_t lim = want ? 2 * k + 16 : 3 * k + 16;
    for (;;) {
        uint8_t *f = sqf_sieve(lim);
        uint64_t c = 0;
        for (uint64_t i = 1; i <= lim; i++) {
            if ((int)f[i] == want) {
                c++;
                if (c == k) {
                    free(f);
                    return i;
                }
            }
        }
        free(f);
        lim *= 2;
    }
}

static uint64_t rec(uint64_t n) {
    if (n <= 1) return 1;
    uint8_t *f = sqf_sieve(n);
    uint64_t sq = 0;
    for (uint64_t i = 1; i <= n; i++) sq += f[i];
    bool isq = f[n] != 0;
    free(f);
    if (!isq) {
        uint64_t j = n - sq;
        uint64_t v = rec(j);
        return nth_kind(1 + v, 1);
    } else {
        uint64_t v = rec(sq - 1);
        return nth_kind(v, 0);
    }
}

seq_t a(idx_t n) {
    return (seq_t)rec((uint64_t)n);
}
