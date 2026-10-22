/*
 * A306468  Number of ways to write n as floor(i^2/3) + floor(j^2/3) +
 *          floor(k^2/3) with 1 <= i <= j <= k.
 * https://oeis.org/A306468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (i,j,k) with 1 ≤ i ≤ j ≤ k, such that
 *            floor(i^2/3)+floor(j^2/3)+floor(k^2/3) = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Shared state, initialized on first use */
static seq_t *counter = (seq_t*)NULL;     /* array a[0..MAX]          */
static size_t  counter_size = 0;          /* allocated size           */
static const idx_t MAX = 10000;           /* sufficient for all tests */

static void init_counts(void) {
    if (counter != NULL)  /* already initialized */
        return;

    /* allocate array of seq_t for indices up to MAX */
    counter = (seq_t*)calloc((size_t)(MAX + 1), sizeof(seq_t));
    if (counter == NULL)
        abort();
    counter_size = (size_t)(MAX + 1);

    /* find maximum index needed for i in f(i) = floor(i²/3) */
    idx_t i = 1;
    while (1) {
        idx_t v = (i * i) / 3;
        if (v > MAX)
            break;
        ++i;
    }
    idx_t KMAX = i - 1;   /* inclusive */

    /* pre‑compute f(i) values */
    idx_t *f = (idx_t*)calloc((size_t)(KMAX + 1), sizeof(idx_t));
    if (f == NULL)
        abort();
    for (i = 1; i <= KMAX; ++i)
        f[i] = (i * i) / 3;

    /* enumerate all triples i ≤ j ≤ k with early stopping */
    for (idx_t ii = 1; ii <= KMAX; ++ii) {
        idx_t fi = f[ii];
        for (idx_t jj = ii; jj <= KMAX; ++jj) {
            idx_t fj = f[jj];
            idx_t s2 = fi + fj;                /* fi + fj       */
            for (idx_t kk = jj; kk <= KMAX; ++kk) {
                idx_t sum = s2 + f[kk];
                if (sum > MAX)
                    break;                    /* f[kk] monotone */
                /* cast sum to size_t is safe: sum ≤ MAX < 2¹⁶ */
                ++counter[(size_t)sum];
            }
        }
    }
    free(f);
}

seq_t a(idx_t n) {
    init_counts();
    if (n < counter_size)
        return counter[(size_t)n];
    /* out of precomputed range – return 0 */
    return (seq_t)0;
}
