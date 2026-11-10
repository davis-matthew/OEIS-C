/*
 * A371087  a(n) = 1 if A003557(n) divides A276086(n), otherwise a(n) = 0,
 *          where A003557(k) is k divided by its largest squarefree divisor,
 *          and A276086 is the primorial base exp-function.
 * https://oeis.org/A371087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A003557(n) | A276086(n), else 0. Since A276086(n) =
 *            Prod prime(k+1)^d_k (d_k = primorial-base digits of n), this
 *            holds iff for every prime p = prime(k+1) with p^e || n, e>=2, we
 *            have d_k >= e-1
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 141 terms through n = 9324 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const unsigned A371087_pr[24] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89};

seq_t a(idx_t n)
{
    uint64_t d[24];
    int len = 0;
    uint64_t m = (uint64_t)n;
    while (m > 0 && len < 24) {
        d[len] = m % A371087_pr[len];
        m /= A371087_pr[len];
        len++;
    }

    uint64_t t = (uint64_t)n;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p != 0) continue;
        uint64_t e = 0;
        while (t % p == 0) { t /= p; e++; }
        if (e >= 2) {
            uint64_t dig = 0;
            for (int k = 0; k < len; k++) {
                if (A371087_pr[k] == p) { dig = d[k]; break; }
            }
            if (dig < e - 1) return (seq_t)0;
        }
    }
    return (seq_t)1;
}
