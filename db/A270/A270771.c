/*
 * A270771  Total sum of the sizes of all blocks with minimal element 8 in all
 *          set partitions of {1,2,...,n}.
 * https://oeis.org/A270771
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n-8} binom(n-8, k) * (k+1) * B_{n-1-k} * where
 *            B_m is the m‑th Bell number (number of set partitions of a set
 *            of size m). * a(8) is the first term of the sequence
 * Range:     u8 none, u16 n <= 10, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *bell = NULL;   /* bell[0..]  */
static size_t bell_len = 0;  /* largest index computed (inclusive) */
static size_t bell_cap = 0;  /* allocated capacity */

/* Ensure bell[0..max] are ready.  */
static void ensure_bell(size_t max)
{
    if (max <= bell_len) return;          /* already computed */

    /* grow capacity */
    if (bell_cap < max + 1) {
        size_t new_cap = bell_cap ? bell_cap : 1;
        while (new_cap <= max) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                 /* out of memory */
        bell = tmp;
        bell_cap = new_cap;
    }

    /* B(0) = 1 by definition; initialise if first time */
    if (bell_len == 0) {
        bell[0] = (seq_t)1;
        bell_len = 0;
    }

    /* compute missing Bell numbers */
    for (size_t i = bell_len + 1; i <= max; ++i) {
        seq_t sum = 0;
        seq_t binom = 1;                  /* C(i-1,0) */
        for (size_t k = 0; k < i; ++k) {
            seq_t term = bell[k] * binom;
            sum += term;
            if (k + 1 < i) {
                binom = binom * (seq_t)(i - 1 - k);
                binom = binom / (seq_t)(k + 1);
            }
        }
        bell[i] = sum;
    }
    bell_len = max;
}

seq_t a(idx_t n)
{
    if (n < 8) return 0;                       /* values below 8 are irrelevant */

    size_t nn = (size_t)n;
    size_t need_bell = nn - 1;                 /* we need B(0..n-1) */

    /* initialise / grow storage as needed */
    if (!bell) {
        bell = (seq_t *)malloc(sizeof(seq_t));
        if (!bell) exit(1);
        bell_cap = 1;
        bell[0] = (seq_t)1;
        bell_len = 0;
    }
    ensure_bell(need_bell);

    /* compute the sum a(n) */
    size_t t = nn - 8;                         /* upper limit for k */

    seq_t ans = 0;
    seq_t binom = 1;                           /* C(t,0) */

    for (size_t k = 0; k <= t; ++k) {
        seq_t term = bell[need_bell - k];      /* B(n-1-k) */
        term = term * (seq_t)(k + 1);          /* (k+1)*B(n-1-k) */
        term = term * binom;                   /* *C(t,k)   */
        ans += term;

        if (k + 1 <= t) {
            binom = binom * (seq_t)(t - k);
            binom = binom / (seq_t)(k + 1);
        }
    }
    return ans;
}
