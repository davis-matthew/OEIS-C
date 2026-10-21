/*
 * A293208  Let b be the lexicographically earliest sequence of positive terms
 *          such that the function f defined by f(n) = Sum_{k=1..n} (i^k *
 *          b(k)) for any n >= 0 is injective (where i denotes the imaginary
 *          unit), and b(n) != b(n+1) and b() != b(n+2) for any n > 0; a(n) =
 *          real part of f(n).
 * https://oeis.org/A293208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = real part of f(n), where f(n) = Σ_{k=1..n} (i^k * b(k)),
 *            b(k) is the lexicographically earliest positive integer sequence
 *            with b(k) ≠ b(k+1) and b(k) ≠ b(k+2) and such that the complex
 *            numbers f(n) are all distinct
 * Range:     i8 n <= 9804, i16 n <= 9804, i32 n <= 9804, i64 n <= 9804,
 *            i128 n <= 9804, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    seq_t real;
    seq_t imag;
} point_t;

static point_t *f_vals = NULL;   /* f_values[0..max_n] */
static seq_t  *b_vals = NULL;   /* b_values[0..max_n] (b[0] dummy) */
static idx_t   max_n   = 0;      /* highest n that has been computed */
static size_t  cap     = 0;      /* allocation capacity */

/* ensure that the arrays are large enough for indices 0..need */
static void ensure_capacity(idx_t need)
{
    if (cap <= (size_t)need) {
        size_t newcap = cap ? cap * 2 : 128;
        while (newcap <= (size_t)need) newcap <<= 1;
        point_t *newf = (point_t *)realloc(f_vals, newcap * sizeof(point_t));
        seq_t  *newb = (seq_t  *)realloc(b_vals, newcap * sizeof(seq_t));
        if (!newf || !newb) exit(1);  /* allocation failed */
        f_vals = newf;
        b_vals = newb;
        cap    = newcap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* f(0) = 0 → a(0) = 0 */

    if (!f_vals) {                 /* first call: initialise */
        ensure_capacity(1);
        f_vals[0].real = 0;
        f_vals[0].imag = 0;
        b_vals[0]      = 0;        /* dummy, unused */
        max_n          = 0;
    }

    if (n <= max_n) return f_vals[n].real;

    ensure_capacity(n);

    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t cand = 1;
        /* skip values that would violate the no‑consecutive‑equal rule */
        while (((k > 1) && cand == b_vals[k - 1]) ||
               ((k > 2) && cand == b_vals[k - 2]))
            ++cand;

        while (true) {
            /* compute tentative f(k) based on f(k-1) */
            seq_t new_real = f_vals[k - 1].real;
            seq_t new_imag = f_vals[k - 1].imag;
            idx_t mod = k % 4ULL;   /* i^k cycle: 1, i, -1, -i */

            if (mod == 1) {
                new_imag += cand;          /*  i  */
            } else if (mod == 2) {
                new_real -= cand;          /* -1  */
            } else if (mod == 3) {
                new_imag -= cand;          /* -i  */
            } else { /* mod == 0 */
                new_real += cand;          /* 1   */
            }

            /* check that this f(k) is distinct from all previous ones */
            bool dup = false;
            for (idx_t t = 0; t < k; ++t) {
                if (f_vals[t].real == new_real && f_vals[t].imag == new_imag) {
                    dup = true;
                    break;
                }
            }

            if (!dup) {   /* found a valid b(k) */
                f_vals[k].real = new_real;
                f_vals[k].imag = new_imag;
                b_vals[k]      = cand;
                break;
            }

            /* otherwise try the next candidate (ensuring no consecutive repeats) */
            ++cand;
            while (((k > 1) && cand == b_vals[k - 1]) ||
                   ((k > 2) && cand == b_vals[k - 2]))
                ++cand;
        }
    }

    max_n = n;
    return f_vals[n].real;
}
