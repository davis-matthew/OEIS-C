/*
 * A359109  Row sums of the accumulated Stirling2 triangle A359107.
 * https://oeis.org/A359109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)*Bell(n) - Bell(n+1) Bell(0) = 1, Bell(m+1) =
 *            Σ_{k=0}^{m} C(m,k) * Bell(k) (m ≥ 0)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *Bell = NULL;          /* array of Bell numbers */
static idx_t Bell_len = 0;          /* number of computed Bell numbers (0 .. Bell_len-1) */
static size_t Bell_cap = 0;        /* allocated capacity */

/* helper : grow the Bell array to at least new_cap elements */
static void ensure_capacity(size_t new_cap) {
    if (Bell_cap >= new_cap) return;
    size_t old_cap = Bell_cap;
    Bell_cap = new_cap;
    Bell = (seq_t *)realloc(Bell, Bell_cap * sizeof(seq_t));
    if (!Bell) exit(1);
    /* initialise the newly added elements to zero */
    for (size_t i = old_cap; i < Bell_cap; ++i) Bell[i] = (seq_t)0;
}

/* compute Bell numbers up to index max_needed (inclusive) */
static void ensure_bell(idx_t max_needed) {
    if (Bell_len > max_needed) return;          /* already have enough */

    /* grow capacity if necessary */
    if (Bell_len) {
        size_t needed_cap = Bell_len;
        if (needed_cap <= (size_t)max_needed) {
            size_t new_cap = Bell_cap ? Bell_cap : 2;
            while (new_cap <= (size_t)max_needed) new_cap <<= 1;
            ensure_capacity(new_cap);
        }
    } else {
        /* first allocation */
        size_t new_cap = 2;                     /* at least for indices 0 and 1 */
        while (new_cap <= (size_t)max_needed) new_cap <<= 1;
        Bell = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!Bell) exit(1);
        for (size_t i = 0; i < new_cap; ++i) Bell[i] = (seq_t)0;
        Bell_cap = new_cap;
        Bell[0] = (seq_t)1;                     /* Bell(0) = 1 */
        Bell_len = 1;
    }

    /* compute missing Bell numbers */
    for (idx_t m = Bell_len; m <= max_needed; ++m) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;                  /* C(m-1,0) for the recurrence of Bell(m) */
        for (idx_t k = 0; k < m; ++k) {
            sum += bin * Bell[k];
            if (k < m - 1) {
                /* update bin to C(m-1, k+1) */
                bin = bin * (seq_t)(m - 1 - k) / (seq_t)(k + 1);
            }
        }
        Bell[m] = sum;
    }
    Bell_len = max_needed + 1;
}

/* Public sequence function */
seq_t a(idx_t n) {
    /* we need Bell(n) and Bell(n+1) */
    ensure_bell(n + 1);
    seq_t res = ((seq_t)(n + 2)) * Bell[n] - Bell[n + 1];
    return res;
}
