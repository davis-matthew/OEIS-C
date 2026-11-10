/*
 * A341708  Negative terms in A341707, in order of appearance (and negated).
 * https://oeis.org/A341708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = -A341707(m) where m is the n-th index with A341707(m) <
 *            0; A341707(m) = y(k) - Sum_{i<k} b_i*y(i), y(j) =
 *            reverse_decimal(2^j), k = floor(log2(m)). Computed by a linear
 *            scan over m
 * Range:     u8 n <= 48, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-10)
 * Helpers:   pow2, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"
#include "../utils/reverse_digits.h"

static seq_t A341708_Y[64];
static bool A341708_init = false;

static void A341708_setup(void)
{
    if (A341708_init) return;
    for (unsigned i = 0; i < 64; ++i)
        A341708_Y[i] = reverse_digits(pow2((idx_t)i));
    A341708_init = true;
}

seq_t a(idx_t n)
{
    static idx_t cm = 0;      /* number of negatives found so far */
    static idx_t cn = 0;      /* last index scanned */
    static bool have = false;
    static seq_t cv;

    A341708_setup();

    idx_t count, m;
    if (have && n == cm)
        return cv;
    if (have && n > cm) {
        count = cm;
        m = cn + 1;
    } else {
        count = 0;
        m = 1;
    }

    for (;; ++m) {
        unsigned k = 0;
        while (k < 63 && (m >> (k + 1)) != 0)
            ++k;
        seq_t P = A341708_Y[k];
        seq_t S = 0;
        for (unsigned i = 0; i < k; ++i)
            if ((m >> i) & 1)
                S += A341708_Y[i];
        if (S > P) {
            ++count;
            if (count == n) {
                cm = count;
                cn = m;
                cv = S - P;
                have = true;
                return cv;
            }
        }
    }
}
