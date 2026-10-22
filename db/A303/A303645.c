/*
 * A303645  Duplicate of A064364.
 * https://oeis.org/A303645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the n‑th positive integer when the integers are sorted by
 *            increasing sum of their prime factors (with multiplicity), where
 *            ties are broken by the smaller integer
 * Range:     i8 n <= 24, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes a permutation of the integers up to a
   fixed limit using a linear sieve to compute the sum of prime factors.
   The precomputation is performed lazily on the first call to a(n).  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

struct Pair {
    int sum;   /* sum of prime factors (with multiplicity) */
    idx_t val; /* the integer itself */
};

/* comparison function for qsort: first compare sums, then values */
static int pair_cmp(const void *a, const void *b)
{
    const struct Pair *pa = (const struct Pair *)a;
    const struct Pair *pb = (const struct Pair *)b;
    if (pa->sum != pb->sum) return pa->sum - pb->sum;
    if (pa->val < pb->val) return -1;
    if (pa->val > pb->val) return 1;
    return 0;
}

/* precomputation data */
static bool built = false;
static const size_t LIMIT = 2000000;       /* enough for n up to ~10^4 */
static seq_t *perm = NULL;                 /* 1‑based permutation */

/* perform the precomputation once */
static void build_sequence(void)
{
    /* smallest prime factor for each integer */
    int *spf = (int *)calloc(LIMIT + 1, sizeof(int));
    if (!spf) return; /* allocation failed, leave built=false */

    /* array of primes collected by the linear sieve */
    int *primes = (int *)malloc(LIMIT * sizeof(int));
    if (!primes) { free(spf); return; }
    size_t pc = 0;

    /* linear sieve */
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!spf[i]) {
            spf[i] = (int)i;
            primes[pc++] = (int)i;
        }
        for (size_t j = 0; j < pc &&
             (size_t)primes[j] <= (size_t)spf[i] &&
             i * primes[j] <= LIMIT; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }

    /* compute sum of prime factors with multiplicity */
    int *sum = (int *)calloc(LIMIT + 1, sizeof(int));
    if (!sum) { free(spf); free(primes); return; }
    sum[1] = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        sum[i] = sum[i / spf[i]] + spf[i];
    }

    /* build array of (sum, value) pairs */
    struct Pair *pairs = (struct Pair *)malloc(LIMIT * sizeof(struct Pair));
    if (!pairs) { free(spf); free(primes); free(sum); return; }

    for (size_t i = 1; i <= LIMIT; ++i) {
        pairs[i - 1].sum = sum[i];
        pairs[i - 1].val = (idx_t)i;
    }

    /* sort the pairs */
    qsort(pairs, LIMIT, sizeof(struct Pair), pair_cmp);

    /* allocate the permutation array (1‑based indexing) */
    perm = (seq_t *)malloc((LIMIT + 1) * sizeof(seq_t));
    if (!perm) {
        free(spf); free(primes); free(sum); free(pairs); return;
    }
    for (size_t i = 0; i < LIMIT; ++i) {
        perm[i + 1] = (seq_t)pairs[i].val;
    }

    /* clean up temporary arrays */
    free(spf);
    free(primes);
    free(sum);
    free(pairs);

    built = true;
}

/* public routine requested by the specification */
seq_t a(idx_t n)
{
    if (!built) {
        build_sequence();
        if (!built) return (seq_t)0; /* in case precomputation failed */
    }
    if (n == 0 || n > LIMIT) return (seq_t)0;
    return perm[n];
}
