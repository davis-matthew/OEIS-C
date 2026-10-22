/*
 * A346356  Numbers that are the sum of six fifth powers in exactly one way.
 * https://oeis.org/A346356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers that are the sum of six fifth powers in exactly one way.
   1‑based indexing: a(1) is the first such number. */
#include <stdlib.h>
#include <stdbool.h>

/* static helper to compare seq_t values for qsort  */
static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t v1 = *(const seq_t *)p1;
    seq_t v2 = *(const seq_t *)p2;
    return (v1 < v2) ? -1 : (v1 > v2) ? 1 : 0;
}

/* static storage for the unique sums */
static seq_t *unique_vals = NULL;
static size_t unique_len  = 0;
static bool precomputed   = false;

/* generate all sums of six 5th powers using numbers 1..B
   and return the number of sums that occur exactly once.
   If 'storage' is non‑NULL, it receives a freshly allocated array
   containing those unique sums sorted increasingly.
*/
static size_t generate_with_B(idx_t B, seq_t **storage)
{
    /* pre‑compute fifth powers of 1..B */
    seq_t *base = (seq_t *)malloc((B + 1) * sizeof(seq_t));
    if (!base) exit(1);
    for (idx_t i = 1; i <= B; ++i) {
        seq_t val = (seq_t)i;
        for (int k = 1; k < 5; ++k) val *= (seq_t)i;
        base[i] = val;
    }

    /* number of non‑decreasing 6‑tuples:  C(B+5,6) */
    size_t Ncomb = 1;
    for (int i = 1; i <= 6; ++i)
        Ncomb = Ncomb * (B + 5 - (i - 1)) / i;

    seq_t *sums = (seq_t *)malloc(Ncomb * sizeof(seq_t));
    if (!sums) { free(base); exit(1); }

    size_t idx = 0;
    for (idx_t a1 = 1; a1 <= B; ++a1)
        for (idx_t a2 = a1; a2 <= B; ++a2)
            for (idx_t a3 = a2; a3 <= B; ++a3)
                for (idx_t a4 = a3; a4 <= B; ++a4)
                    for (idx_t a5 = a4; a5 <= B; ++a5)
                        for (idx_t a6 = a5; a6 <= B; ++a6)
                            sums[idx++] = base[a1] + base[a2] + base[a3] +
                                          base[a4] + base[a5] + base[a6];

    free(base);
    /* sort all sums */
    qsort(sums, Ncomb, sizeof(seq_t), cmp_seq_t);

    /* extract sums that occur exactly once */
    seq_t *tmp = (seq_t *)malloc(Ncomb * sizeof(seq_t));
    if (!tmp) { free(sums); exit(1); }

    size_t uniq = 0;
    for (size_t i = 0; i < Ncomb; ) {
        size_t j = i + 1;
        while (j < Ncomb && sums[j] == sums[i]) ++j;
        if (j - i == 1) tmp[uniq++] = sums[i];
        i = j;
    }
    free(sums);

    /* shrink array to needed size */
    seq_t *res = (seq_t *)malloc(uniq * sizeof(seq_t));
    if (!res) { free(tmp); exit(1); }
    for (size_t i = 0; i < uniq; ++i) res[i] = tmp[i];
    free(tmp);
    *storage = res;
    return uniq;
}

/* ensure that we have computed at least 'need' unique sums */
static void ensure_precomputed(idx_t need)
{
    if (precomputed && unique_len >= (size_t)need) return;

    /* attempt with a modest B; if not enough, increase B */
    idx_t B = 20;
    while (1) {
        seq_t *tmp = NULL;
        size_t cnt = generate_with_B(B, &tmp);
        if (cnt >= (size_t)need) {
            /* success – store the new results */
            free(unique_vals);
            unique_vals = tmp;
            unique_len  = cnt;
            precomputed = true;
            return;
        }
        /* not enough – clean up and try a larger B */
        free(tmp);
        B *= 2;
        if (B > 100) { /* safety limit – should never happen for n≤10000 */
            break;
        }
    }
    /* if we reach here, something went wrong – abort */
    exit(1);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_precomputed(n);
    if (n > unique_len) return (seq_t)0;
    return unique_vals[n - 1];
}
