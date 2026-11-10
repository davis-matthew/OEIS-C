/*
 * A286729  a(n)=position of the first occurrence of a local maximum equal to
 *          2n in A001223, n>1.
 * https://oeis.org/A286729
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. Sieve primes p(1)=2, p(2)=3, ... and
 *            return the first k with g(k) = 2n, g(k) > g(k-1) and g(k) >
 *            g(k+1), where g(k) = A001223(k) = p(k+1) - p(k). The sieve limit
 *            doubles until a match is found
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 45 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t a286729_search(uint64_t target)
{
    uint64_t lim = (uint64_t)1 << 16;
    for (;;) {
        unsigned char *comp = (unsigned char *)malloc((size_t)lim + 1);
        if (!comp) return 0;
        memset(comp, 0, (size_t)lim + 1);
        uint64_t i, j;
        for (i = 2; i * i <= lim; i++)
            if (!comp[i])
                for (j = i * i; j <= lim; j += i) comp[j] = 1;
        /* 0-based primes q[0]=2, q[1]=3, ...; A001223(k) = q[k] - q[k-1] */
        uint64_t p0 = 0, p1 = 0, p2 = 0; /* q[cnt-3], q[cnt-2], q[cnt-1] */
        uint64_t cnt = 0;                /* primes seen so far */
        uint64_t found = 0;
        for (i = 2; i <= lim; i++) {
            if (comp[i]) continue;
            uint64_t p3 = i;             /* q[cnt] */
            if (cnt >= 3) {
                uint64_t gk = p2 - p1;   /* A001223(cnt-1) */
                uint64_t gprev = p1 - p0;/* A001223(cnt-2) */
                uint64_t gnext = p3 - p2;/* A001223(cnt)   */
                if (gk == target && gk > gprev && gk > gnext) {
                    found = cnt - 1;
                    break;
                }
            }
            p0 = p1; p1 = p2; p2 = p3;
            cnt++;
        }
        free(comp);
        if (found) return found;
        lim *= 2;
    }
}

seq_t a(idx_t n)
{
    return (seq_t)a286729_search((uint64_t)n * 2);
}
