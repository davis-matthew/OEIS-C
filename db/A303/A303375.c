/*
 * A303375  Numbers of the form a^5 + b^6, with integers a, b > 0.
 * https://oeis.org/A303375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A303375 – numbers of the form a^5 + b^6, a,b > 0.
    The function returns the n‑th smallest such number (n ≥ 1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Size limits for the precomputed search grid.  200×200 ≈ 40 000 sums
   well exceed the 10 000‑term requirement for typical tests. */
#define A_MAX 200
#define B_MAX 200

/* Global cache for the sorted, unique set of a^5 + b^6 values. */
static bool initialized = false;
static seq_t *uniq_vals = NULL;
static size_t uniq_len = 0;

/* Comparator used by qsort. */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Pre‑compute all a^5 + b^6 for 1 ≤ a ≤ A_MAX, 1 ≤ b ≤ B_MAX,
   then sort and deduplicate into uniq_vals.  This runs only on the
   first call to a(). */
static void init_sequence(void)
{
    const size_t total = (size_t)A_MAX * (size_t)B_MAX;
    /* Allocate temporary array for raw sums */
    seq_t *sums = (seq_t *)malloc(total * sizeof(seq_t));
    if (!sums) { /* OOM; in a contest setting this will crash */ }

    /* Pre‑compute powers of a and b. */
    seq_t pow5[A_MAX + 1];
    seq_t pow6[B_MAX + 1];

    pow5[0] = 0;               /* not used */
    for (idx_t a = 1; a <= A_MAX; ++a) {
        seq_t t = (seq_t)a;
        seq_t t5 = t * t;       /* a² */
        t5 = t5 * t;             /* a³ */
        t5 = t5 * t;             /* a⁴ */
        t5 = t5 * t;             /* a⁵ */
        pow5[a] = t5;
    }

    pow6[0] = 0;               /* not used */
    for (idx_t b = 1; b <= B_MAX; ++b) {
        seq_t t = (seq_t)b;
        seq_t t6 = t * t;       /* b² */
        t6 = t6 * t;             /* b³ */
        t6 = t6 * t;             /* b⁴ */
        t6 = t6 * t;             /* b⁵ */
        t6 = t6 * t;             /* b⁶ */
        pow6[b] = t6;
    }

    /* Build the list of sums. */
    size_t idx = 0;
    for (idx_t a = 1; a <= A_MAX; ++a) {
        for (idx_t b = 1; b <= B_MAX; ++b) {
            sums[idx++] = pow5[a] + pow6[b];
        }
    }

    /* Sort the sums. */
    qsort(sums, idx, sizeof(seq_t), cmp_seq);

    /* Deduplicate into uniq_vals. */
    uniq_vals = (seq_t *)malloc(idx * sizeof(seq_t));
    uniq_len = 0;
    for (size_t i = 0; i < idx; ++i) {
        if (uniq_len == 0 || sums[i] != uniq_vals[uniq_len - 1]) {
            uniq_vals[uniq_len++] = sums[i];
        }
    }

    free(sums);
    initialized = true;
}

/* Public function: return the n-th element of the sequence. */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    if (n == 0) return 0;            /* sequence not defined for index 0 */
    if ((size_t)n > uniq_len) return 0;  /* out of precomputed range */
    return uniq_vals[n - 1];
}
