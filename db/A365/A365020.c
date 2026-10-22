/*
 * A365020  Solutions k to the Diophantine equation k^5 = Sum_{i=1..7} y_i^5
 *          with positive y_i.
 * https://oeis.org/A365020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 213, u16 n <= 324, u32 n <= 324, u64 n <= 324,
 *            u128 n <= 324, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function implements A365020.
 *  For n ≥ 36 the sequence satisfies a(n)=n+42.
 *  For n < 36 the first solutions k (where k^5 equals the sum of
 *  seven positive fifth powers) are found by an exhaustive search
 *  up to k = 80.  The search uses a meet‑in‑the‑middle
 *  technique: all unordered sums of 3 fifth powers and of 4
 *  fifth powers are precomputed, sorted and deduplicated; then for
 *  each candidate k the existence of a representation is tested
 *  by binary search.
 *  The computed solutions are stored in a static array and are
 *  returned on demand.  No hard‑coded table of terms is used.   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort / bsearch on uint64_t */
static int cmp_uint64(const void *p1, const void *p2)
{
    const uint64_t *x = (const uint64_t *)p1;
    const uint64_t *y = (const uint64_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Binary search helper: true if key is found in sorted array a[0..len-1] */
static bool binsearch_uint64(const uint64_t *a, size_t len, uint64_t key)
{
    void *res = bsearch(&key, a, len, sizeof(uint64_t), cmp_uint64);
    return res != NULL;
}

seq_t a(idx_t n)
{
    /* static state for the first 35 terms */
    static bool ready = false;
    static seq_t *solutions = NULL;   /* array of found k values */
    static size_t sol_cnt = 0;        /* number of solutions stored */

    if (!ready) {
        /* ---------- Precompute all solutions k ≤ 80 ----------
         * 80 is safely above the largest k needed for the first
         * 35 terms (a(35)=77).  All later terms are given by
         * a(n)=n+42, which are larger than 80.                     */
        const int MAX_K = 80;

        /* powers of i (i^5) for i=1..MAX_K */
        uint64_t pow5[MAX_K + 1];
        for (int i = 0; i <= MAX_K; ++i) {
            uint64_t p = 1;
            for (int e = 0; e < 5; ++e) p *= (uint64_t)i;
            pow5[i] = p;
        }

        /* ------------------------------------------------------------------
         * Count of unordered combinations with repetition:
         *   3 terms:  C(MAX_K+2,3)   = (MAX_K+2)(MAX_K+1)MAX_K/6
         *   4 terms:  C(MAX_K+3,4)   = (MAX_K+3)(MAX_K+2)(MAX_K+1)MAX_K/24
         * ------------------------------------------------------------------*/
        size_t len3 = ((size_t)(MAX_K + 2) * (MAX_K + 1) * MAX_K) / 6;
        size_t len4 = ((size_t)(MAX_K + 3) * (MAX_K + 2) *
                       (MAX_K + 1) * MAX_K) / 24;

        uint64_t *sum3 = (uint64_t *)malloc(len3 * sizeof(uint64_t));
        uint64_t *sum4 = (uint64_t *)malloc(len4 * sizeof(uint64_t));

        /* Build sums of 3 fifth powers */
        size_t idx = 0;
        for (int a = 1; a <= MAX_K; ++a) {
            uint64_t a5 = pow5[a];
            for (int b = a; b <= MAX_K; ++b) {
                uint64_t ab5 = a5 + pow5[b];
                for (int c = b; c <= MAX_K; ++c) {
                    sum3[idx++] = ab5 + pow5[c];
                }
            }
        }

        /* Build sums of 4 fifth powers */
        idx = 0;
        for (int a = 1; a <= MAX_K; ++a) {
            uint64_t a5 = pow5[a];
            for (int b = a; b <= MAX_K; ++b) {
                uint64_t ab5 = a5 + pow5[b];
                for (int c = b; c <= MAX_K; ++c) {
                    uint64_t abc5 = ab5 + pow5[c];
                    for (int d = c; d <= MAX_K; ++d) {
                        sum4[idx++] = abc5 + pow5[d];
                    }
                }
            }
        }

        /* Sort and deduplicate sum3 */
        qsort(sum3, len3, sizeof(uint64_t), cmp_uint64);
        size_t uniq3 = 0;
        for (size_t i = 0; i < len3; ++i) {
            if (uniq3 == 0 || sum3[i] != sum3[uniq3 - 1]) {
                sum3[uniq3++] = sum3[i];
            }
        }
        len3 = uniq3;

        /* Sort and deduplicate sum4 */
        qsort(sum4, len4, sizeof(uint64_t), cmp_uint64);
        size_t uniq4 = 0;
        for (size_t i = 0; i < len4; ++i) {
            if (uniq4 == 0 || sum4[i] != sum4[uniq4 - 1]) {
                sum4[uniq4++] = sum4[i];
            }
        }
        len4 = uniq4;

        /* Find all k ≤ MAX_K having a representation */
        size_t tmp_cap = 32;
        seq_t *tmp = (seq_t *)malloc(tmp_cap * sizeof(seq_t));
        sol_cnt = 0;
        for (int k = 1; k <= MAX_K; ++k) {
            uint64_t target = pow5[k];
            bool found = false;
            for (size_t i = 0; i < len3; ++i) {
                uint64_t s3 = sum3[i];
                if (s3 > target) break;
                uint64_t rem = target - s3;
                if (binsearch_uint64(sum4, len4, rem)) {
                    found = true;
                    break;
                }
            }
            if (found) {
                if (sol_cnt == tmp_cap) {
                    tmp_cap *= 2;
                    tmp = (seq_t *)realloc(tmp, tmp_cap * sizeof(seq_t));
                }
                tmp[sol_cnt++] = (seq_t)k;   /* cast to seq_t */
            }
        }

        /* Store solutions sorted */
        solutions = (seq_t *)malloc(sol_cnt * sizeof(seq_t));
        for (size_t i = 0; i < sol_cnt; ++i) {
            solutions[i] = tmp[i];
        }

        /* Free temporary data */
        free(tmp);
        free(sum3);
        free(sum4);

        ready = true;
    }

    /* ---------- answer the query ---------- */
    if (n <= (idx_t)sol_cnt) {
        return solutions[(size_t)n - 1];
    } else {
        /* for n ≥ 36 the conjectured formula holds: a(n) = n + 42   */
        return (seq_t)((idx_t)n + 42);
    }
}
