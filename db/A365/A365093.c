/*
 * A365093  Smallest k such that A365092(k) = n.
 * https://oeis.org/A365093
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = least k with A365092(k) = n; A365092(1)=0, A365092(2)=2,
 *            A365092(p)=A365092(p-1)+1 for odd primes p,
 *            A365092(m*n)=A365092(m)+A365092(n) for coprime m,n,
 *            A365092(p^e)=A365092(p)+A365092(e). Computed by a sieve over
 *            growing limits
 * Range:     u8 n <= 19, u16 n <= 40, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 49 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A365093_MAXV 512

static uint64_t A365093_first[A365093_MAXV];
static uint64_t A365093_done_limit = 0;

static void A365093_build(uint64_t L)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
    uint16_t *f = (uint16_t *)calloc((size_t)(L + 1), sizeof(uint16_t));
    if (!spf || !f) { free(spf); free(f); return; }
    for (uint64_t i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i <= L / i) {
                for (uint64_t j = i * i; j <= L; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    for (int i = 0; i < A365093_MAXV; i++) A365093_first[i] = 0;
    f[1] = 0;
    for (uint64_t k = 2; k <= L; k++) {
        uint32_t p = spf[k];
        if (p == k) {
            f[k] = (k == 2) ? 2 : (uint16_t)(f[k - 1] + 1);
        } else {
            uint64_t m = k;
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            f[k] = (uint16_t)(f[p] + f[e] + f[m]);
        }
        if (f[k] < A365093_MAXV && A365093_first[f[k]] == 0)
            A365093_first[f[k]] = k;
    }
    A365093_done_limit = L;
    free(spf);
    free(f);
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    if (n >= A365093_MAXV) return 0;
    if (A365093_done_limit == 0) A365093_build(4096);
    while (A365093_first[n] == 0 && A365093_done_limit < ((uint64_t)1 << 32)) {
        A365093_build(A365093_done_limit * 4);
    }
    return (seq_t)A365093_first[n];
}
