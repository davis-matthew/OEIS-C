/*
 * A377584  E.g.f.: exp(Sum_{k>=1} A326305(k) * x^k/k).
 * https://oeis.org/A377584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   e.g.f. A(x)=exp(Sum_{k>=1} A326305(k)*x^k/k); a(0)=1, a(n) =
 *            Sum_{k=1..n} A326305(k) * (n-1)!/(n-k)! * a(n-k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-10)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/totient.h"
#include "../utils/mem.h"

static seq_t A326305_a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    seq_t phi_n = totient(n);
    if (n & 1ULL) {
        return phi_n;
    }
    seq_t phi_half = totient(n >> 1);
    return phi_n - phi_half;
}

seq_t a(idx_t n)
{
    idx_t m, k;
    seq_t *c = mem_new(seq_t, (size_t)n + 2);
    seq_t *v = mem_new(seq_t, (size_t)n + 2);
    seq_t result;
    for (k = 1; k <= n; k++) {
        c[k] = A326305_a(k);
    }
    v[0] = (seq_t)1;
    for (m = 1; m <= n; m++) {
        seq_t sum = (seq_t)0;
        seq_t f = (seq_t)1; /* (m-1)!/(m-k)! */
        for (k = 1; k <= m; k++) {
            sum += c[k] * f * v[m - k];
            f = f * (seq_t)(m - k);
        }
        v[m] = sum;
    }
    result = v[n];
    mem_free(c);
    mem_free(v);
    return result;
}
