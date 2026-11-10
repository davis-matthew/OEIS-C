/*
 * A391344  Odd semiprimes k = p*q such that neither k = A048720(A065621(p),x)
 *          nor k = A048720(A065621(q),x) for any x.
 * https://oeis.org/A391344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd semiprime k=p*q (p<=q odd primes) such that
 *            neither A065621(p) nor A065621(q) divides k in GF(2)[x]
 *            (carryless product); found by sieve + search
 * Range:     u8 n <= 39, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9228 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A391344_rev(uint64_t n)
{
    uint64_t low = n & (~n + 1);
    return n ^ ((n - low) << 1);
}

static int A391344_blen(uint64_t x)
{
    int b = 0;
    while (x) { b++; x >>= 1; }
    return b;
}

/* does polynomial r divide polynomial k over GF(2)? */
static int A391344_divides(uint64_t k, uint64_t r)
{
    int dr = A391344_blen(r);
    int dk;
    while ((dk = A391344_blen(k)) >= dr)
        k ^= r << (dk - dr);
    return k == 0;
}

seq_t a(idx_t n)
{
    uint64_t limit = 1024;
    for (;;) {
        uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
        uint64_t i, j, cnt = 0;
        if (!spf) return 0;
        for (i = 2; i <= limit; i++) {
            if (spf[i] == 0) {
                for (j = i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
        for (i = 9; i <= limit; i += 2) {
            uint64_t p = spf[i], q = i / p;
            if (p < 3 || spf[q] != q) continue;
            if (A391344_divides(i, A391344_rev(p))) continue;
            if (A391344_divides(i, A391344_rev(q))) continue;
            cnt++;
            if (cnt == (uint64_t)n) {
                free(spf);
                return (seq_t)i;
            }
        }
        free(spf);
        limit *= 2;
    }
}
