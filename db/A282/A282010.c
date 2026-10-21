/*
 * A282010  Number of ways to partition Turan graph T(2n,n) into connected
 *          components.
 * https://oeis.org/A282010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = Σ_{j=0..n} ((-1)^(n-j)) * Bell(2*j) * binomial(n,j)
 *   where Bell(k) is the k‑th Bell number.
 *
 * The function memoises Bell numbers up to 2*n.
 * It uses the provided seq_t arithmetic and helper functions.
 */

#include <stddef.h>                     /* for size_t */
#include "../utils/binomial.h"          /* seq_t binomial(idx_t n, idx_t k) */
#include "../utils/seq_alloc.h"        /* seq_t *seq_alloc(size_t) */
#include "../utils/seq_realloc.h"      /* seq_t *seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */

static seq_t *bell_arr = NULL;           /* array storing Bell numbers 0..current_max */
static size_t bell_cap = 0;              /* allocated size of bell_arr */

/* ------------------------------------------------------------------ */
/* Ensure that bell_arr contains Bell numbers for indices 0 .. up_to   */
/* ------------------------------------------------------------------ */
static void ensure_bell(idx_t up_to)
{
    /* already have enough? */
    if (up_to + 1 <= bell_cap)
        return;

    size_t new_cap = (size_t)up_to + 1;
    seq_t *old_arr = bell_arr;
    size_t old_cap = bell_cap;

    /* reallocate array */
    bell_arr = seq_realloc(bell_arr, bell_cap, new_cap);
    bell_cap = new_cap;

    /* compute missing Bell numbers */
    for (idx_t i = (idx_t)old_cap; i <= up_to; ++i) {
        if (i == 0) {
            bell_arr[0] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        seq_t c   = 1;                      /* binom(i-1,0) */
        idx_t k_max = i - 1;
        for (idx_t k = 0; k <= k_max; ++k) {
            sum += bell_arr[k] * c;
            if (k < k_max) {
                c = c * (seq_t)(k_max - k);
                c = c / (idx_t)(k + 1);
            }
        }
        bell_arr[i] = sum;
    }
}

/* ------------------------------------------------------------------ */
/* OEIS A282010: Number of partitions of T(2n,n) into connected parts */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t two_n = n + n;
    ensure_bell(two_n);                  /* Bell numbers up to 2*n available */

    seq_t result = 0;
    for (idx_t j = 0; j <= n; ++j) {
        seq_t binom = binomial(n, j);    /* C(n,j) */
        seq_t term  = bell_arr[2 * j] * binom;
        if (((n - j) & 1) == 0)
            result += term;
        else
            result -= term;
    }
    return result;
}
