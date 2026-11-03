/*
 * A300547  a(n) = [x^n] Product_{d|n} (1 - x^d).
 * https://oeis.org/A300547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = coefficient of x^n in ∏_{d|n} (1 - x^d)
 * Range:     i8 n <= 629, i16 n <= 3149, i32 n <= 20789, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 102 terms through n = 9205 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *coeff = NULL;      /* buffer reused for successive calls */
static size_t coeff_cap = 0;     /* capacity of the buffer */

seq_t a(idx_t n)
{
    /* Handle the offset zero */
    if (n == 0) {
        return (seq_t)1;
    }

    size_t ns = (size_t)n;

    /* Ensure the buffer is large enough */
    if (coeff_cap < ns + 1) {
        mem_free(coeff);
        coeff = mem_new(seq_t, (ns + 1));
        coeff_cap = ns + 1;
    }

    /* Initialise the polynomial coefficients */
    for (size_t i = 0; i <= ns; ++i) {
        coeff[i] = (seq_t)0;
    }
    coeff[0] = (seq_t)1;

    /* For each divisor d of n multiply the current polynomial by (1 - x^d) */
    for (size_t d = 1; d <= ns / d; ++d) {
        if (ns % d == 0) {
            size_t d1 = d;
            for (size_t s = ns; s >= d1; --s) {
                coeff[s] -= coeff[s - d1];
                if (s == d1) break;  /* avoid unsigned underflow */
            }
            if (d1 != ns / d) {          /* process the paired divisor */
                size_t d2 = ns / d;
                for (size_t s = ns; s >= d2; --s) {
                    coeff[s] -= coeff[s - d2];
                    if (s == d2) break;
                }
            }
        }
    }

    return coeff[ns];
}
