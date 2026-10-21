/*
 * A255065  Expansion of x * psi(x^5) * f(-x^10) / f(-x^4, -x^6) in powers of
 *          x where psi(), f() are Ramanujan theta functions.
 * https://oeis.org/A255065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1, 5*k*(k‑1)/2 ≤ n‑1} p(n‑1 – 5*k*(k‑1)/2) where
 *            p(m) is the number of partitions of m into parts congruent to 4
 *            or 6 modulo 10 (i.e., parts 4,6,14,16,24,26,...)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Efficiently compute the partitions by dynamic programming on the
    allowed part sizes.  The computation is memoised so repeated calls
    do not recompute the partition array.  */

#include <stdlib.h>   /* for malloc, free */

static seq_t *part = NULL;          /* array of partition numbers */
static size_t part_cap = 0;          /* allocated size */

/* Recompute the partition values for all n up to 'nmax' inclusive.   */
static void recompute_part(idx_t nmax) {
    size_t sz = (size_t)nmax + 1;
    seq_t *newp = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!newp) abort();                 /* insufficient memory          */
    for (size_t i = 0; i < sz; ++i) newp[i] = 0;
    newp[0] = 1;                        /* one way to make zero          */

    /* parts ≡ 4 (mod 10) */
    for (idx_t s = 4; s <= nmax; s += 10)
        for (size_t i = (size_t)s; i < sz; ++i)
            newp[i] += newp[i - s];

    /* parts ≡ 6 (mod 10) */
    for (idx_t s = 6; s <= nmax; s += 10)
        for (size_t i = (size_t)s; i < sz; ++i)
            newp[i] += newp[i - s];

    free(part);
    part = newp;
    part_cap = sz;
}

/* Ensure that the partition array is computed up to 'nmax'.            */
static void ensure_part(idx_t nmax) {
    if (part == NULL || part_cap <= (size_t)nmax)
        recompute_part(nmax);
}

/*  Returns the n-th term of OEIS A255065  */
seq_t a(idx_t n) {
    if (n == 0)               /* offset 1: n<1 is undefined; return 0 */
        return 0;

    idx_t N = n - 1;          /* shift by the leading x of the GF   */
    ensure_part(N);

    seq_t res = 0;
    for (idx_t k = 1;; ++k) {
        idx_t term = 5 * k * (k - 1) / 2;   /* 5*k*(k-1)/2  */
        if (term > N) break;
        res += part[N - term];
    }
    return res;
}
