/*
 * A368821  Number of compositions of n into 2 refactorable parts.
 * https://oeis.org/A368821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368821 – number of compositions of n into two refactorable parts.
   Formula: a(n) = Σ_{k=1}^{n-1} c(k) * c(n-k),
   where c(k) = 1 iff k is refactorable (k mod τ(k) == 0) and 0 otherwise. */

#include <stdlib.h>

static unsigned char *is_refactorable = NULL; /* 1 if refactorable, 0 otherwise */
static idx_t ref_max = 0;                    /* largest index for which the array is valid */

/* Ensure that is_refactorable[1 .. n] is ready. Recomputes from scratch if needed. */
static void ensure_refactorable(idx_t n)
{
    if (n <= ref_max) return;

    idx_t new_n = n;
    unsigned char *new_ref = (unsigned char *)calloc(new_n + 1, sizeof(unsigned char));
    if (!new_ref) return;           /* allocation failure – unlikely for n < 10000 */

    idx_t *divcnt = (idx_t *)calloc(new_n + 1, sizeof(idx_t));
    if (!divcnt) { free(new_ref); return; }

    /* Sieve divisor counts. */
    for (idx_t i = 1; i <= new_n; ++i) {
        for (idx_t j = i; j <= new_n; j += i) {
            ++divcnt[j];
        }
    }

    /* Mark refactorable numbers. */
    for (idx_t i = 1; i <= new_n; ++i) {
        if (divcnt[i] != 0 && (i % divcnt[i]) == 0) {
            new_ref[i] = 1;
        }
    }

    free(is_refactorable);
    free(divcnt);
    is_refactorable = new_ref;
    ref_max = new_n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Ensure values up to n-1 are available; allocating up to n is harmless. */
    ensure_refactorable(n > 0 ? n : 0);

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        if (is_refactorable[k] && is_refactorable[n - k]) {
            count += 1;
        }
    }
    return count;
}
