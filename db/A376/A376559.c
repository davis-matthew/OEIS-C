/*
 * A376559  Second differences of consecutive perfect powers (A001597). First
 *          differences of A053289.
 * https://oeis.org/A376559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = P[n+2] - 2*P[n+1] + P[n] , where P is the sorted list of
 *            unique perfect powers (A001597). a(1) = 1, a(2) = -3, … as in
 *            the OEIS entry
 * Range:     i8 none, i16 none, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* compare function for qsort */
static int cmp_seq(const void *a, const void *b)
{
    seq_t v1 = *(const seq_t *)a;
    seq_t v2 = *(const seq_t *)b;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

/* static storage for the perfect–power list */
static seq_t *pp   = NULL;   /* array of perfect powers */
static size_t pp_len = 0;    /* number of elements */

/* generate the list of unique perfect powers up to MAX_VAL
   (chosen large enough for n up to 10 000). */
static void init_pp(void)
{
    if (pp) return;  /* already initialized */

    /* provisional capacity */
    size_t cap = 4096;
    pp = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!pp) exit(1);   /* out of memory */
    pp_len = 0;

    /* include 1 explicitly */
    pp[pp_len++] = 1;

    /* upper bound on perfect powers to generate */
    const idx_t MAX_VAL = 200000000ULL;  /* 2·10^8 > required for n≈10⁴ */

    /* generate powers for base ≥ 2 */
    for (idx_t base = 2; ; ++base) {
        seq_t val = (seq_t)base * base;   /* start with exponent 2 */
        if (val > MAX_VAL) break;

        while (val <= MAX_VAL) {
            if (pp_len == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(pp, cap * sizeof(seq_t));
                if (!tmp) exit(1);
                pp = tmp;
            }
            pp[pp_len++] = val;

            if (val > MAX_VAL / base) break;   /* would overflow or exceed limit */
            val *= base;   /* next exponent */
        }
    }

    /* sort and remove duplicates */
    qsort(pp, pp_len, sizeof(seq_t), cmp_seq);
    size_t uniq = 0;
    for (size_t i = 0; i < pp_len; ++i) {
        if (uniq == 0 || pp[i] != pp[uniq - 1]) {
            pp[uniq++] = pp[i];
        }
    }
    pp_len = uniq;
    pp = (seq_t *)realloc(pp, pp_len * sizeof(seq_t));
}

/* sequence A376559: second differences of consecutive perfect powers */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n = 0 under the OEIS offset */

    init_pp();   /* ensure the perfect‑power list is ready */

    /* ensure that we have pp[n+1] (0‑based indexing) */
    if (n + 2 > pp_len) {
        /* should not happen for n ≤ 10 000 with the chosen MAX_VAL */
        return 0;
    }

    seq_t p0 = pp[n - 1];          /* P[n]   (since pp is 0‑based) */
    seq_t p1 = pp[n];              /* P[n+1] */
    seq_t p2 = pp[n + 1];          /* P[n+2] */

    return p2 - (p1 + p1) + p0;    /* p2 - 2*p1 + p0 */
}
