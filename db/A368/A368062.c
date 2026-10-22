/*
 * A368062  Numbers k such that k = A257850(k) + A257297(k).
 * https://oeis.org/A368062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k such that k = A257850(k) + A257297(k),
 *            where A257850(k) = first digit of k × remaining digits,
 *            A257297(k) = (all digits except the last) × last digit
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 24, u64 n <= 56, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence begins:
   a(1) = 0, a(2) = 36, a(3) = 655, a(4) = 1258, ... */

#include <stdlib.h>
#include <stdbool.h>

/* Comparator for qsort on seq_t values. */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Cache of computed terms. */
    static seq_t *term = NULL;
    static size_t term_len = 0, term_cap = 0;
    /* Cache of powers of ten: pow10[i] = 10^i. */
    static seq_t *pow10 = NULL;
    static size_t pow10_len = 0;
    /* Current number of digits being processed. */
    static idx_t curr_len = 1;

    while (term_len < n) {
        /* Ensure pow10 array large enough for current length. */
        if (curr_len >= 2) {
            size_t required = curr_len - 2;
            while (pow10_len <= required) {
                size_t new_cap = (pow10_len == 0) ? 2 : pow10_len << 1;
                seq_t *tmp = (seq_t *)realloc(pow10, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;
                pow10 = tmp;
                if (pow10_len == 0) {
                    pow10[0] = (seq_t)1;
                    pow10[1] = (seq_t)10;
                    pow10_len = 2;
                }
                for (size_t i = pow10_len; i < new_cap; ++i)
                    pow10[i] = pow10[i - 1] * (seq_t)10;
                pow10_len = new_cap;
            }
        }

        if (curr_len == 1) {
            /* The only 1‑digit solution is 0. */
            if (term_len == term_cap) {
                size_t new_cap = (term_cap == 0) ? 16 : term_cap << 1;
                seq_t *tmp = (seq_t *)realloc(term, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;
                term = tmp;
                term_cap = new_cap;
            }
            term[term_len++] = (seq_t)0;
            curr_len++;
            continue;
        }

        /* Generate all solutions with `curr_len` digits. */
        const seq_t *Pptr = (curr_len >= 2) ? &pow10[curr_len - 2] : NULL;
        const seq_t P = (curr_len >= 2) ? *Pptr : (seq_t)1; /* 10^(m-2) */

        seq_t local[90];
        size_t local_cnt = 0;

        for (int d1 = 1; d1 <= 9; ++d1) {
            for (int dm = 0; dm <= 9; ++dm) {
                /* Denominator: 10*d1 + dm - 10 (positive). */
                int denom = 10 * d1 + dm - 10;
                if (denom == 0) continue; /* division by zero case */

                /* temp1 = d1 * P * (10 - dm) */
                seq_t temp1 = (seq_t)d1 * P * (seq_t)(10 - dm);
                /* temp2 = dm * (d1 - 1) */
                seq_t temp2 = (seq_t)dm * (seq_t)(d1 - 1);
                seq_t num = temp1 - temp2; /* numer positive */

                if (num % denom != 0) continue; /* not integer solution */

                seq_t r = num / denom;
                if (r >= P) continue; /* r must fit in P */

                /* Build k = d1*10*P + r*10 + dm */
                seq_t k = (seq_t)d1 * 10 * P + r * (seq_t)10 + (seq_t)dm;
                local[local_cnt++] = k;
            }
        }

        if (local_cnt > 0) {
            /* Sort local results to maintain increasing order. */
            qsort(local, local_cnt, sizeof(seq_t), cmp_seq_t);

            for (size_t i = 0; i < local_cnt; ++i) {
                if (term_len == term_cap) {
                    size_t new_cap = (term_cap == 0) ? 16 : term_cap << 1;
                    seq_t *tmp = (seq_t *)realloc(term, new_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;
                    term = tmp;
                    term_cap = new_cap;
                }
                term[term_len++] = local[i];
            }
        }

        curr_len++;
    }

    return term[n - 1];
}
