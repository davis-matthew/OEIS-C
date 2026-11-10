/*
 * A370881  Numbers k >= 1 such that k = A008472(k)*A066328(k).
 * https://oeis.org/A370881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   search for k with k = A008472(k)*A066328(k), where A008472 is
 *            the sum of the distinct primes dividing k and A066328 is the sum
 *            of their indices; a(n) is the n-th such k (found by a sieve plus
 *            trial-division search up to 2^20)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define A370881_LIM (1u << 20)

static uint32_t A370881_terms[64];
static int A370881_cnt = 0;
static int A370881_done = 0;

static void A370881_init(void)
{
    if (A370881_done) return;
    A370881_done = 1;
    uint32_t *pi = (uint32_t *)calloc(A370881_LIM + 1, sizeof(uint32_t));
    uint8_t *comp = (uint8_t *)calloc(A370881_LIM + 1, 1);
    uint32_t *pr = (uint32_t *)malloc(sizeof(uint32_t) * 90000);
    if (!pi || !comp || !pr) { free(pi); free(comp); free(pr); return; }
    size_t np = 0;
    for (uint32_t i = 2; i <= A370881_LIM; i++) {
        if (!comp[i]) {
            pr[np++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= A370881_LIM; j += i) comp[j] = 1;
        }
        pi[i] = (uint32_t)np;
    }
    for (uint32_t k = 2; k <= A370881_LIM && A370881_cnt < 64; k++) {
        uint32_t m = k;
        uint64_t s = 0, t = 0;
        for (size_t i = 0; i < np; i++) {
            uint32_t p = pr[i];
            if ((uint64_t)p * p > m) break;
            if (m % p == 0) {
                s += p; t += i + 1;
                while (m % p == 0) m /= p;
            }
        }
        if (m > 1) { s += m; t += pi[m]; }
        if (s * t == k) A370881_terms[A370881_cnt++] = k;
    }
    free(pi); free(comp); free(pr);
}

seq_t a(idx_t n)
{
    A370881_init();
    if (n < 1 || n > (idx_t)A370881_cnt) return 0;
    return (seq_t)A370881_terms[n - 1];
}
