/*
 * A382869  Numbers k >= 1 such that A018804(k) is a Fibonacci number
 *          (A000045).
 * https://oeis.org/A382869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search k = 1, 2, 3, ... and keep k when A018804(k) is a
 *            Fibonacci number. A018804 is computed for all k at once with a
 *            linear sieve, using that it is multiplicative with A018804(p^e)
 *            = p*A018804(p^(e-1)) + p^e - p^(e-1)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A382869_CAP 4096
#define A382869_MAXL ((uint64_t)1 << 26)

static uint64_t A382869_found[A382869_CAP];
static size_t A382869_cnt = 0;
static uint64_t A382869_lim = 0;

static bool A382869_isfib(uint64_t v, const uint64_t *fib, int nf)
{
    int lo = 0, hi = nf - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (fib[mid] == v) return true;
        if (fib[mid] < v) lo = mid + 1; else hi = mid - 1;
    }
    return false;
}

static void A382869_search(uint64_t L)
{
    uint64_t fib[94];
    int nf;
    fib[0] = 1; fib[1] = 2; nf = 2;
    while (nf < 92) { fib[nf] = fib[nf - 1] + fib[nf - 2]; nf++; }

    uint64_t *P = (uint64_t *)malloc((size_t)(L + 1) * sizeof(uint64_t));
    uint32_t *q = (uint32_t *)malloc((size_t)(L + 1) * sizeof(uint32_t));
    uint32_t *primes = (uint32_t *)malloc((size_t)(L / 2 + 100) * sizeof(uint32_t));
    size_t np = 0;
    if (!P || !q || !primes) { free(P); free(q); free(primes); return; }
    P[0] = 0; q[0] = 0;
    P[1] = 1; q[1] = 1;
    A382869_cnt = 0;
    A382869_found[A382869_cnt++] = 1;
    for (uint64_t i = 2; i <= L; i++) {
        if (q[i] == 0) {
            q[i] = (uint32_t)i;
            P[i] = 2 * i - 1;
            primes[np++] = (uint32_t)i;
        }
        for (size_t j = 0; j < np; j++) {
            uint64_t p = primes[j];
            uint64_t x = i * p;
            if (x > L) break;
            if (i % p == 0) {
                uint64_t qx = (uint64_t)q[i] * p;
                q[x] = (uint32_t)qx;
                if (qx == x) {
                    /* i = p^(e-1), x = p^e */
                    P[x] = p * P[i] + i * (p - 1);
                } else {
                    P[x] = P[qx] * P[x / qx];
                }
                break;
            } else {
                q[x] = (uint32_t)p;
                P[x] = P[i] * P[p];
            }
        }
        if (A382869_cnt < A382869_CAP && A382869_isfib(P[i], fib, nf))
            A382869_found[A382869_cnt++] = i;
    }
    free(P); free(q); free(primes);
    A382869_lim = L;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A382869_lim == 0) A382869_search((uint64_t)1 << 21);
    while (A382869_cnt < (size_t)n && A382869_cnt < A382869_CAP
           && A382869_lim != 0 && A382869_lim < A382869_MAXL) {
        uint64_t L = A382869_lim * 2;
        if (L > A382869_MAXL) L = A382869_MAXL;
        A382869_search(L);
        if (A382869_lim != L) break;
    }
    if ((size_t)n > A382869_cnt) return 0;
    return (seq_t)A382869_found[n - 1];
}
