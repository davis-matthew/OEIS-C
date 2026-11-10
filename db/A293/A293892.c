/*
 * A293892  Restricted growth sequence transform of A083722.
 * https://oeis.org/A293892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of A083722, computed
 *            by a sieve: each value is labelled by the order of its first
 *            occurrence, starting at 1
 * Range:     u8 n <= 324, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 115 terms through n = 9835 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    size_t N = (size_t)n;
    unsigned char *comp = (unsigned char *)calloc(N + 2, 1);
    uint64_t *lpf = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    uint64_t *nxt = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    uint64_t *lp  = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    uint64_t *lab = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    uint64_t *key = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    if (!comp || !lpf || !nxt || !lp || !lab || !key) exit(1);

    for (size_t i = 2; i <= N; i++) {
        if (!comp[i]) {
            for (size_t j = i; j <= N; j += i) {
                if (j != i) comp[j] = 1;
                lpf[j] = i;
            }
        }
    }
    /* nxt[i] = smallest prime > i (N+1 sentinel if none up to N) */
    nxt[N] = N + 1;
    for (size_t i = N; i-- > 0;) {
        if (i + 1 >= 2 && !comp[i + 1]) nxt[i] = i + 1;
        else nxt[i] = nxt[i + 1];
    }
    /* lp[m] = largest prime <= m */
    for (size_t m = 2; m <= N; m++)
        lp[m] = comp[m] ? lp[m - 1] : m;

    uint64_t count = 1;
    lab[1] = 1;
    for (size_t m = 2; m <= N; m++) {
        uint64_t p = lp[m];
        uint64_t q = nxt[lpf[m]];
        if (q > p) {
            lab[m] = 1;
            key[m] = 0;
            continue;
        }
        key[m] = q;
        uint64_t found = 0;
        for (size_t k = (size_t)p; k < m; k++) {
            if (key[k] == q) { found = lab[k]; break; }
        }
        if (found) lab[m] = found;
        else lab[m] = ++count;
    }
    seq_t res = (seq_t)lab[N];
    free(comp); free(lpf); free(nxt); free(lp); free(lab); free(key);
    return res;
}
