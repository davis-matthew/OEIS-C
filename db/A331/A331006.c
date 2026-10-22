/*
 * A331006  Numbers k for which there is no number m < k such that U(k) =
 *          U(m), where U(k) = phi(k)/k for k > 2 even, phi(k)/(k-1) for k > 1
 *          odd, U(2) = 1.
 * https://oeis.org/A331006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: generate k consecutively, keep a table of all reduced U(k)
   values seen so far (as a rational number num/den).  A k is added to
   the sequence a until we have produced n terms.  All arithmetic is
   performed in 64‑bit unsigned integers, which is sufficient for any
   n that fits in the test data (n ≤ 10 000 gives k < 100 000).  This
   avoids dependence on the arbitrary‑precision implementation of gcd
   in the utils library. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/calc_phi.h"
#include "../utils/gcd_uint64.h"

/* type for storing seen fractions (reduced rational numbers) */
typedef struct {
    uint64_t num;
    uint64_t den;
} frac_uint64_t;

/* fast gcd for 64‑bit integers */

/* dynamic arrays for accepted k values and seen fractions */
static seq_t *seq      = NULL;
static size_t seq_len  = 0, seq_cap = 0;

static frac_uint64_t *frac = NULL;
static size_t frac_len  = 0, frac_cap = 0;

/* ensure capacity of seq array */
static void grow_seq(size_t needed)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        if (new_cap < needed) new_cap = needed;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq) exit(EXIT_FAILURE);
        seq_cap = new_cap;
    }
}

/* ensure capacity of frac array */
static void grow_frac(size_t needed)
{
    if (frac_len >= frac_cap) {
        size_t new_cap = frac_cap ? frac_cap * 2 : 4;
        if (new_cap < needed) new_cap = needed;
        frac = (frac_uint64_t *)realloc(frac, new_cap * sizeof(frac_uint64_t));
        if (!frac) exit(EXIT_FAILURE);
        frac_cap = new_cap;
    }
}

/* search function for a fractional value (reduced) */
static bool fraction_seen(uint64_t num, uint64_t den)
{
    for (size_t i = 0; i < frac_len; ++i) {
        if (frac[i].num == num && frac[i].den == den)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    /* Generate terms until we have n accepted k values */
    uint64_t k = 2;                       /* start at 2 */
    while (seq_len < (size_t)n) {
        seq_t phi = calc_phi(k);

        uint64_t num, den;
        if (k == 2) {
            num = 1;
            den = 1;
        } else if ((k & 1) == 0) {         /* even k > 2 */
            num = (uint64_t)phi;
            den = k;
        } else {                           /* odd k > 1 */
            num = (uint64_t)phi;
            den = k - 1;
        }

        /* reduce the fraction */
        uint64_t g = gcd_uint64(num, den);
        num /= g;
        den /= g;

        /* accept k if this reduced fraction is new */
        if (!fraction_seen(num, den)) {
            grow_frac(frac_len + 1);
            frac[frac_len].num = num;
            frac[frac_len].den = den;
            ++frac_len;

            grow_seq(seq_len + 1);
            seq[seq_len++] = (seq_t)k;
        }

        ++k;
    }

    return seq[n - 1];
}
