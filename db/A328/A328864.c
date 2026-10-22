/*
 * A328864  For any three-digit number k = hdu, f(k) = (h+d+u) + (h*d+d*u+u*h)
 *          + (h*d*u). This sequence consists of the numbers k for which the
 *          ratio k/f(k) is an integer.
 * https://oeis.org/A328864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: the nth 3‑digit integer k where k(f(k))⁻¹ is an integer.
   f(k) = (h+d+u)+(h*d+d*u+u*h)+(h*d*u) for k = hdu. */
#include <stdbool.h>
#include <stddef.h>

static bool initialized = false;
static seq_t terms[900];
static size_t term_count = 0;

/* Populate terms[] the first time a() is called */
static void build_terms(void)
{
    for (uint64_t k = 100; k <= 999; ++k) {
        uint64_t u = k % 10;
        uint64_t d = (k / 10) % 10;
        uint64_t h = k / 100;
        uint64_t f = h + d + u + h*d + d*u + u*h + h*d*u;
        if (f != 0 && k % f == 0) {
            terms[term_count++] = (seq_t)k;
        }
    }
}

/* Return the n-th element of the sequence (1‑based).  If n is out of
   bounds, zero is returned. */
seq_t a(idx_t n)
{
    if (!initialized) {
        build_terms();
        initialized = true;
    }
    if (n == 0 || n > term_count) {
        return 0;
    }
    return terms[n - 1];
}
