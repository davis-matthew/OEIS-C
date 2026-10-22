/*
 * A358133  Triangle read by rows whose n-th row lists the first differences
 *          of the n-th composition in standard order (row n of A066099).
 * https://oeis.org/A358133
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = n-th term of the triangle read by rows whose * rows
 *            contain the successive differences of the n-th composition * in
 *            standard graded reverse‑lexicographic order (A066099)
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t  s_next_k      = 1;          /* next composition index (k)       */
static idx_t  s_next_index  = 2;          /* last defined index of a(n)       */
static seq_t *s_cache       = NULL;       /* memoized values                  */
static size_t s_cache_cap   = 0;          /* allocated capacity (indices 0…cap-1) */

/* push n-th term into cache */
static void push_val(seq_t val) {
    /* grow cache if needed */
    if (s_next_index + 1 >= (idx_t)s_cache_cap) {
        size_t new_cap = s_cache_cap ? s_cache_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(s_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);               /* abort on allocation failure */
        s_cache = tmp;
        s_cache_cap = new_cap;
    }
    ++s_next_index;
    s_cache[s_next_index] = val;
}

/* compute all successive differences of the composition for given k
   and push them into the cache */
static void process_k(idx_t k) {
    /* diff_arr stores the successive differences of the positions
       of the set bits in k (positions are 1‑based).  Its length equals
       popcount(k). */
    int diff_arr[64];
    int len = 0;
    int prev = 0;
    idx_t temp = k;
    int bit = 0;
    while (temp) {
        if (temp & 1ULL) {
            int pos = bit + 1;             /* positions are 1‑based */
            diff_arr[len++] = pos - prev;
            prev = pos;
        }
        temp >>= 1;
        ++bit;
    }
    /* produce the differences of the reversed diff_arr (i.e. of the
       composition parts).  For a length m array the reversed array is
       diff_arr[m-1], diff_arr[m-2], … , diff_arr[0].  The successive
       differences are therefore: diff_arr[m-2]−diff_arr[m-1],
       diff_arr[m-3]−diff_arr[m-2], … . */
    for (int i = 0; i + 1 < len; ++i) {
        int cur  = diff_arr[len - 1 - i];
        int next = diff_arr[len - 2 - i];
        seq_t d = (seq_t)(next - cur);
        push_val(d);
    }
}

/* public function: a(n) */
seq_t a(idx_t n) {
    if (n < 3) return 0;             /* values below offset are irrelevant */

    /* initialize cache on first call */
    if (s_cache == NULL) {
        seq_t *tmp = (seq_t *)calloc(32, sizeof(seq_t));
        if (!tmp) exit(1);
        s_cache = tmp;
        s_cache_cap = 32;
        /* first three entries are unused/zero */
    }

    /* generate terms until we have up to index n */
    while (s_next_index < n) {
        process_k(s_next_k);
        ++s_next_k;
    }
    return s_cache[n];
}
