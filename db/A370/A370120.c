/*
 * A370120  a(n) = A276085(A370117(n)), where A370117(n) is the denominator of
 *          n/A276086(A003415(n)), A003415 is the arithmetic derivative,
 *          A276086 is the primorial base exp-function, and A276085 is its
 *          inverse, the primorial base log-function.
 * https://oeis.org/A370120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276085(A370117(n))
 * Range:     u8 n <= 95, u16 n <= 9530, u32 n <= 9530, u64 n <= 9530,
 *            u128 n <= 9530, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n <= 2)
        return 0;

    /* ---- Constants: first 13 primes and their cumulative products ---- */
    static const uint64_t PRIMES[13] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41};
    static seq_t CUM[13];
    static int initialized = 0;
    if (!initialized) {
        CUM[0] = 1;
        for (int i = 1; i < 13; ++i)
            CUM[i] = CUM[i - 1] * (seq_t)PRIMES[i - 1];
        initialized = 1;
    }

    /* ---------- 1. arithmetic derivative d of n ---------- */
    seq_t d = 0;
    uint64_t fac_pr[32], fac_ex[32];
    size_t fc = 0;
    factor_u64((uint64_t)n, fac_pr, fac_ex, &fc);
    for (size_t i = 0; i < fc; ++i) {
        uint64_t prime = fac_pr[i];
        uint64_t exp = fac_ex[i];
        d += ((seq_t)n / (seq_t)prime) * (seq_t)exp;
    }
    if (d == 0)
        return 0;  /* should not happen for n>=3, but keeps code safe */

    /* ---------- 2. convert d into primorial‑base digits a[i] ---------- */
    size_t i_start = 0;
    while (i_start + 1 < 13 && CUM[i_start + 1] <= d)
        ++i_start;

    seq_t a_digit[13] = {0};
    seq_t rem = d;
    for (size_t i = i_start; ; --i) {
        a_digit[i] = rem / CUM[i];
        rem -= a_digit[i] * CUM[i];
        if (i == 0) break;
    }

    /* ---------- 3. exponents of n for primes up to i_start ---------- */
    seq_t exp_num[13] = {0};
    for (size_t j = 0; j < fc; ++j) {
        uint64_t p = fac_pr[j];
        for (size_t idx = 0; idx <= i_start; ++idx) {
            if (PRIMES[idx] == p) {
                exp_num[idx] = (seq_t)fac_ex[j];
                break;
            }
        }
    }

    /* ---------- 4. compute index of the denominator ---------- */
    seq_t result = 0;
    for (size_t i = 0; i <= i_start; ++i) {
        if (a_digit[i] > exp_num[i]) {
            seq_t diff = a_digit[i] - exp_num[i];
            result += diff * CUM[i];
        }
    }
    return result;
}
