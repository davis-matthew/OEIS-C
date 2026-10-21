/*
 * A276847  Expansion of eta(q^2) * eta(q^4) * eta(q^6) * eta(q^12) in powers
 *          of q.
 * https://oeis.org/A276847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 2322, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of q^n in
 *     q * Π_{k>0} (1 - q^{2k})(1 - q^{4k})(1 - q^{6k})(1 - q^{12k})
 * This is implemented by directly expanding the infinite product.
 * The expansion for the part without the leading q is
 *     Π_{k∈{2,4,6,12}} Π_{m≥1} (1 - q^{k m}).
 * Coefficients are computed lazily up to the largest index requested.
 * The algorithm has O(n²) time for a maximum index n and uses only
 * integer arithmetic on the provided seq_t type.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *coeff = NULL;      /* coefficients of the product up to coeff_size-1 */
static size_t coeff_size = 0;    /* number of coefficients stored */

/* Multipliers in the Euler product */
static const idx_t ks[4] = {2, 4, 6, 12};

/* Recompute all coefficients up to 'limit' (inclusive) */
static void recompute_up_to(idx_t limit)
{
    size_t new_size = (size_t)limit + 1;
    /* Allocate and zero all entries */
    seq_t *tmp = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    tmp[0] = (seq_t)1;  /* initial value of the empty product */

    /* For each multiplier k, process all multiples e = k, 2k, 3k, ... ≤ limit */
    for (int i = 0; i < 4; ++i) {
        idx_t k = ks[i];
        for (idx_t e = k; e <= limit; e += k) {
            for (idx_t j = limit; j >= e; --j) {
                tmp[j] -= tmp[j - e];
            }
        }
    }

    /* Replace old array by the new one */
    if (coeff) free(coeff);
    coeff = tmp;
    coeff_size = new_size;
}

/* Ensure coefficients up to 'idx' (0‑based) are available */
static void ensure_up_to(idx_t idx)
{
    if ((size_t)(idx + 1) <= coeff_size) return;
    recompute_up_to(idx);
}

/* External function requested by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 safely */
    idx_t idx = n - 1;              /* coefficient index in the product */
    ensure_up_to(idx);
    return coeff[idx];
}
