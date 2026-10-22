/*
 * A392124  Irregular triangular array read by rows: row n shows the
 *          coefficients in the expansion of the n-th derivative of tan(x) as
 *          a linear combination of products of powers of sec(x)^2 and tan(x).
 * https://oeis.org/A392124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by iterating the derivative rule on tan(x) and
 *            collecting the coefficients of each derivative row in descending
 *            order of the exponent of sec². No closed‑form expression is
 *            known
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

typedef struct {
    idx_t k;        /* exponent of sec^2   */
    idx_t m;        /* exponent of tan     */
    seq_t coeff;    /* coefficient in the expansion */
} term_t;

#define MAX_TERMS_PER_ROW 2048

/* Cache for all terms that have been computed */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Term lists for the current and next derivative rows */
static term_t prev_row[MAX_TERMS_PER_ROW];
static size_t prev_cnt = 0;
static int     initialized = 0;

/* Temporary storage for raw terms before merging */
static term_t raw_terms[MAX_TERMS_PER_ROW];
static term_t next_row[MAX_TERMS_PER_ROW];

/* Ensure the cache has enough capacity for |need| additional terms */
static void ensure_cache_capacity(size_t need)
{
    if (cache_len + need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        while (new_cap < cache_len + need) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Initialise the first row (tan(x) itself) and the cache on first call */
    if (!initialized) {
        cache = (seq_t*)malloc(128 * sizeof(seq_t));
        cache_cap = 128;
        cache[0] = (seq_t)1;      /* a(1) */
        cache_len = 1;

        /* First derivative row: k=0, m=1, coeff=1 */
        prev_cnt = 1;
        prev_row[0].k = 0;
        prev_row[0].m = 1;
        prev_row[0].coeff = (seq_t)1;

        initialized = 1;
    }

    /* If the requested term is already cached, return it immediately */
    if (n <= cache_len) return cache[n-1];

    /* Generate successive rows until the requested index is reached */
    while (n > cache_len) {
        size_t raw_cnt = 0;

        /* Produce raw terms from the previous row using the derivative rule */
        for (size_t i = 0; i < prev_cnt; ++i) {
            idx_t k = prev_row[i].k;
            idx_t m = prev_row[i].m;
            seq_t coeff = prev_row[i].coeff;

            /* Rule: D[(sec²)^k * tan^m] = k*2*(sec²)^k * tan^(m+1)
                                       + m*(sec²)^(k+1) * tan^(m-1) */

            if (k > 0) {                   /* derivative of sec² contributes */
                raw_terms[raw_cnt].k = k;
                raw_terms[raw_cnt].m = m + 1;
                raw_terms[raw_cnt].coeff = coeff * ((seq_t)(k * 2));
                ++raw_cnt;
            }
            if (m > 0) {                   /* derivative of tan contributes */
                raw_terms[raw_cnt].k = k + 1;
                raw_terms[raw_cnt].m = m - 1;
                raw_terms[raw_cnt].coeff = coeff * (seq_t)m;
                ++raw_cnt;
            }
        }

        /* Merge duplicate (k,m) pairs into next_row */
        size_t next_cnt = 0;
        for (size_t i = 0; i < raw_cnt; ++i) {
            idx_t k = raw_terms[i].k;
            idx_t m = raw_terms[i].m;
            seq_t coeff = raw_terms[i].coeff;

            /* Linear search for existing entry */
            size_t j;
            for (j = 0; j < next_cnt; ++j) {
                if (next_row[j].k == k && next_row[j].m == m) {
                    next_row[j].coeff += coeff;
                    break;
                }
            }
            if (j == next_cnt) {           /* new entry */
                next_row[next_cnt].k = k;
                next_row[next_cnt].m = m;
                next_row[next_cnt].coeff = coeff;
                ++next_cnt;
            }
        }

        /* Sort next_row by descending k (ties broken arbitrarily) */
        for (size_t i = 1; i < next_cnt; ++i) {
            term_t key = next_row[i];
            size_t j = i;
            while (j > 0 && next_row[j-1].k < key.k) {
                next_row[j] = next_row[j-1];
                --j;
            }
            next_row[j] = key;
        }

        /* Append the coefficients of this row to the cache */
        ensure_cache_capacity(next_cnt);
        for (size_t i = 0; i < next_cnt; ++i) {
            cache[cache_len++] = next_row[i].coeff;
        }

        /* Prepare for the next iteration */
        memcpy(prev_row, next_row, next_cnt * sizeof(term_t));
        prev_cnt = next_cnt;
    }

    return cache[n-1];
}
