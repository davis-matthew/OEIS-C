/*
 * A391715  Denominators of the convergents given by treating A072193 as
 *          continued fraction coefficients after the leading 0.
 * https://oeis.org/A391715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(0) = 1, a(-1) = 0 (so a(1) =
 *            c(1)), where c(k) = A072193(k) are the continued fraction
 *            coefficients after the leading 0
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 47, u128 n <= 89,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 1812 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *dig = NULL;
    static seq_t *den = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static size_t qlen = 0;
    static idx_t next_k = 1;

    while (len < (size_t)n) {
        idx_t k = next_k++;
        idx_t q = 2;
        while ((q - 1) * q / 2 < k) q++;
        idx_t prev = (q - 2) * (q - 1) / 2;
        idx_t p = k - prev;

        seq_t a1 = (seq_t)p;
        seq_t b1 = (seq_t)q;
        bool skip_zero = true;

        while (b1 != 0) {
            seq_t a_val = a1 / b1;
            seq_t r = a1 % b1;
            if (!(skip_zero && a_val == 0)) {
                if (len == cap) {
                    size_t new_cap = (cap == 0) ? 128 : cap * 2;
                    seq_t *t1 = mem_resize(seq_t, dig, new_cap);
                    if (!t1) abort();
                    dig = t1;
                    seq_t *t2 = mem_resize(seq_t, den, new_cap);
                    if (!t2) abort();
                    den = t2;
                    cap = new_cap;
                }
                dig[len++] = a_val;
            }
            skip_zero = false;
            a1 = b1;
            b1 = r;
        }
    }

    while (qlen < (size_t)n) {
        seq_t v;
        if (qlen == 0) v = dig[0];
        else if (qlen == 1) v = dig[1] * den[0] + 1;
        else v = dig[qlen] * den[qlen - 1] + den[qlen - 2];
        den[qlen++] = v;
    }

    return den[n - 1];
}
