/*
 * A395064  a(1) = 1. For n >= 2, a(n) is the smallest positive integer > 1
 *          such that s^n = p where s is the sum of digits of a(n) (cf.
 *          A007953) and p is the product of digits of a(n) (cf. A007954).
 * https://oeis.org/A395064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A395064: a(1)=1; for n≥2 a(n) is the smallest integer >1 such that
   s^n = p where s is the sum of digits of a(n) and p the product of its
   digits. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;               /* offset a(1)=1 */

    seq_t best_val = 0;
    size_t best_len = SIZE_MAX;
    char *best_buf = NULL;

    for (idx_t s = 2; ; ++s) {

        /* minimal possible digits for this or larger s is ceil(s/9)   */
        unsigned long long min_len_est = (s + 8u) / 9u;
        if (min_len_est > best_len)
            break;                      /* cannot beat current best */

        /* factor s with primes 2,3,5,7 only */
        unsigned long long tmp = s;
        unsigned aE2 = 0, aE3 = 0, aE5 = 0, aE7 = 0;
        while (tmp % 2ULL == 0ULL) { ++aE2; tmp /= 2ULL; }
        while (tmp % 3ULL == 0ULL) { ++aE3; tmp /= 3ULL; }
        while (tmp % 5ULL == 0ULL) { ++aE5; tmp /= 5ULL; }
        while (tmp % 7ULL == 0ULL) { ++aE7; tmp /= 7ULL; }
        if (tmp != 1ULL)               /* contains other prime factor */
            continue;

        idx_t E2 = (idx_t)aE2 * n;
        idx_t E3 = (idx_t)aE3 * n;
        idx_t E5 = (idx_t)aE5 * n;
        idx_t E7 = (idx_t)aE7 * n;

        idx_t base_sum = 2 * E2 + 3 * E3 + 5 * E5 + 7 * E7;
        if (s < base_sum)                /* impossible */
            continue;
        idx_t R = s - base_sum;          /* R = x6 + 2x8 + 3x9 + x1 */

        /* iterate over possible x6 and x9 (x8 is taken maximal) */
        for (idx_t x6 = 0; x6 <= E2 && x6 <= E3 && x6 <= R; ++x6) {
            idx_t remaining1 = R - x6;
            idx_t max_x9_by_exps = (E3 - x6) / 2;
            idx_t max_x9_by_R     = remaining1 / 3;
            idx_t max_x9 = (max_x9_by_exps < max_x9_by_R ? max_x9_by_exps : max_x9_by_R);

            for (idx_t x9 = 0; x9 <= max_x9; ++x9) {
                idx_t remaining2 = remaining1 - 3 * x9;

                /* choose maximal x8 allowed by constraints */
                idx_t max_x8_by_exps = (E2 - x6) / 3;
                idx_t max_x8_by_R     = remaining2 / 2;
                idx_t x8 = (max_x8_by_exps < max_x8_by_R ? max_x8_by_exps : max_x8_by_R);

                /* digit 1 fills the rest of R */
                idx_t x1 = remaining2 - 2 * x8;            /* guaranteed ≥0 */

                /* residual exponents for digits 2 and 3 */
                idx_t y2 = E2 - (x6 + 3 * x8);
                idx_t y3 = E3 - (x6 + 2 * x9);
                if (y2 == (idx_t)-1 || y3 == (idx_t)-1)  /* overflow guard */
                    continue;

                /* determine 4s and 2s from y2, 3s from y3 */
                idx_t x4 = y2 / 2;
                idx_t x2 = y2 % 2;
                idx_t x3 = y3;

                /* total length of the candidate number */
                size_t len = (size_t)x1 + x2 + x3 + x4 + E5 + x6 + E7 + x8 + x9;
                if (len > best_len)
                    continue;

                /* build ascending string representation */
                char *buf = (char *)malloc(len + 1);
                if (!buf)
                    abort();
                char *p = buf;
                for (idx_t i = 0; i < x1; ++i) *p++ = '1';
                for (idx_t i = 0; i < x2; ++i) *p++ = '2';
                for (idx_t i = 0; i < x3; ++i) *p++ = '3';
                for (idx_t i = 0; i < x4; ++i) *p++ = '4';
                for (idx_t i = 0; i < E5; ++i) *p++ = '5';
                for (idx_t i = 0; i < x6; ++i) *p++ = '6';
                for (idx_t i = 0; i < E7; ++i) *p++ = '7';
                for (idx_t i = 0; i < x8; ++i) *p++ = '8';
                for (idx_t i = 0; i < x9; ++i) *p++ = '9';
                *p = '\0';

                /* compute numeric value */
                seq_t val = 0;
                for (char *q = buf; *q; ++q)
                    val = val * 10 + (seq_t)(*q - '0');

                /* decide if this candidate is better than the current best */
                bool replace = false;
                if (len < best_len)
                    replace = true;
                else if (len == best_len && memcmp(buf, best_buf, len) < 0)
                    replace = true;

                if (replace) {
                    free(best_buf);
                    best_buf = buf;
                    best_len = len;
                    best_val = val;
                } else {
                    free(buf);
                }
            }
        }
    }

    free(best_buf);                /* clean up */
    return best_val;
}
