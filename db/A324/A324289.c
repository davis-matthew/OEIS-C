/*
 * A324289  a(n) = A276086(A283477(n)).
 * https://oeis.org/A324289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276086(A283477(n)); A283477(n) = product of
 *            primorial(k+1) over set bits k of n; A276086(m) = prod
 *            prime(k+1)^d_k, d_k = digits of m in primorial base
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 440 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define A324289_LIM 5000
static uint32_t A324289_pr[700];
static int A324289_np = 0;

static void A324289_init(void)
{
    if (A324289_np) return;
    static unsigned char comp[A324289_LIM + 1];
    for (uint32_t i = 2; i <= A324289_LIM; ++i) {
        if (!comp[i]) {
            A324289_pr[A324289_np++] = i;
            for (uint32_t j = i * i; j <= A324289_LIM; j += i) comp[j] = 1;
        }
    }
}

seq_t a(idx_t n)
{
    A324289_init();

    /* m = A283477(n) */
    seq_t m = 1;
    seq_t prim = 1;
    idx_t t = n;
    int pos = 0;
    while (t != 0) {
        prim *= (seq_t)A324289_pr[pos];   /* primorial(pos+1) */
        if (t & 1) m *= prim;
        t >>= 1;
        ++pos;
    }

    /* A276086(m): primorial base digits as exponents */
    seq_t result = 1;
    int k = 0;
    while (m != 0 && k < A324289_np) {
        seq_t p = (seq_t)A324289_pr[k];
        seq_t d = m % p;
        m /= p;
        while (d > 0) {
            result *= p;
            --d;
        }
        ++k;
    }
    return result;
}
