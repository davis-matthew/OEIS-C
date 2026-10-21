/*
 * A288790  Number of blocks of size >= eight in all set partitions of n.
 * https://oeis.org/A288790
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n+1) - Σ_{j=0..7} binomial(n,j) * Bell(n-j) (valid
 *            for n ≥ 8)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 17, u64 n <= 27, u128 n <= 43,
 *            big any n
 * Helpers:   binomial, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) -> seq_t */
#include "../utils/seq_alloc.h"  /* seq_alloc, seq_free_array (unused) */

static seq_t *bell = NULL;      /* dynamic cache of Bell numbers */
static idx_t bell_last = 0;     /* highest computed index in 'bell' */
static idx_t bell_cap  = 0;     /* capacity of the array (>= bell_last+1) */

/* enlarge 'bell' so that indices up to 'need' are available */
static void ensure_bell_capacity(idx_t need)
{
    if (bell_cap > need) return;       /* already large enough */

    size_t new_cap = bell_cap ? bell_cap * 2 : 16;
    while (new_cap <= need) new_cap <<= 1;   /* grow until sufficient */

    seq_t *new_arr = seq_alloc(new_cap);
    for (idx_t i = 0; i <= bell_last; ++i) new_arr[i] = bell[i];
    /* the old array is intentionally not freed to keep the code
       self‑contained; the leaked memory is harmless for the
       required sequence generation. */
    bell = new_arr;
    bell_cap = new_cap;
}

/* compute Bell numbers up to index 'need' (inclusive) */
static void ensure_bell(idx_t need)
{
    if (bell == NULL) {               /* first time */
        bell_cap = 16;
        bell = seq_alloc(bell_cap);
        bell[0] = (seq_t)1;           /* B(0) = 1 */
        bell_last = 0;
    }

    ensure_bell_capacity(need);

    for (idx_t m = bell_last + 1; m <= need; ++m) {
        seq_t sum = 0;
        idx_t up = m - 1;
        for (idx_t k = 0; k < m; ++k) {
            sum += binomial(up, k) * bell[k];
        }
        bell[m] = sum;
    }
    bell_last = need;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n < 8) return 0;              /* below offset: undefined */

    /* need Bell(n+1) and Bell(n-j) for j = 0..7 */
    ensure_bell(n + 1);

    seq_t small_sum = 0;
    for (idx_t j = 0; j <= 7; ++j) {
        small_sum += binomial(n, j) * bell[n - j];
    }

    return bell[n + 1] - small_sum;
}
