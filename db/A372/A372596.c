/*
 * A372596  a(1)=-1; thereafter a(n) is (the number of terms that appear with
 *          a different frequency from that of a(n-1)) minus (the number of
 *          terms that appear with the same frequency).
 * https://oeis.org/A372596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n-1) - 2 * f * cnt_f where f = frequency of a(n-1) among
 *            the first n-1 terms, cnt_f = number of distinct terms that have
 *            frequency f
 * Range:     i8 n <= 171, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper structure to keep each distinct value and its frequency. */
struct Dist {
    seq_t val;
    idx_t freq;
};

static seq_t *seq_arr      = NULL;   /* computed sequence values       */
static size_t  seq_cap     = 0;       /* capacity of seq_arr             */
static size_t  computed_n  = 0;       /* largest n computed so far       */

static struct Dist *distinct_arr = NULL; /* distinct values and freq */
static size_t       distinct_cap = 0;
static size_t       distinct_cnt = 0;    /* number of distinct values */

static idx_t *freq_cnt_arr = NULL;   /* freq_cnt_arr[f] = number of distinct values with frequency f */
static size_t freq_cnt_cap = 0;       /* capacity of freq_cnt_arr */

/* index of the distinct value that equals a(n-1) */
static idx_t last_distinct_index = 0;

/* Ensure seq_arr can hold at least 'need' elements. */
static void ensure_seq_cap(size_t need) {
    if (seq_cap >= need) return;
    if (seq_cap == 0) seq_cap = 1;
    while (seq_cap < need) seq_cap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
}

/* Ensure distinct_arr can hold at least 'need' elements. */
static void ensure_distinct_cap(size_t need) {
    if (distinct_cap >= need) return;
    if (distinct_cap == 0) distinct_cap = 1;
    while (distinct_cap < need) distinct_cap <<= 1;
    distinct_arr = (struct Dist *)realloc(distinct_arr, distinct_cap * sizeof(struct Dist));
}

/* Ensure freq_cnt_arr can hold at least 'need' indices. */
static void ensure_freq_cnt_cap(size_t need) {
    if (freq_cnt_cap >= need) return;
    if (freq_cnt_cap == 0) freq_cnt_cap = 1;
    while (freq_cnt_cap < need) freq_cnt_cap <<= 1;
    freq_cnt_arr = (idx_t *)realloc(freq_cnt_arr, freq_cnt_cap * sizeof(idx_t));
    /* Initialize any new slots to zero. */
    for (size_t i = 0; i < need; ++i)
        if (i >= freq_cnt_cap) freq_cnt_arr[i] = 0;
}

/* Computes a(n) for any n >= 1.  The function is memoized and
   works for arbitrary calling order. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safety: not part of the sequence */

    if (n > computed_n) {
        /* Grow necessary storage. */
        ensure_seq_cap((size_t)n);
        ensure_distinct_cap((size_t)n);
        ensure_freq_cnt_cap((size_t)n + 2);  /* +2 in case freq = n */

        /* Initialize the sequence on the first call. */
        if (computed_n == 0) {
            /* a(1) = -1 */
            seq_arr[0]          = (seq_t)-1;
            distinct_arr[0].val = (seq_t)-1;
            distinct_arr[0].freq = 1;
            distinct_cnt = 1;
            freq_cnt_arr[1] = 1;  /* one distinct value with frequency 1 */
            last_distinct_index = 0;
            computed_n = 1;
        }

        /* Compute terms up to required n. */
        for (idx_t cur = computed_n + 1; cur <= n; ++cur) {
            /* Frequency of the previous term a(cur-1). */
            idx_t old_freq = distinct_arr[last_distinct_index].freq;

            /* Number of distinct values that have that frequency. */
            idx_t cnt_f = freq_cnt_arr[old_freq];

            /* Total number of terms that have that frequency. */
            idx_t t_n = old_freq * cnt_f;

            /* a(cur) = (cur-1) - 2 * t_n. */
            seq_t a_cur = (seq_t)((seq_t)(cur - 1) - (seq_t)(2 * t_n));
            seq_arr[cur - 1] = a_cur;

            /* Update the data structures for the new term a_cur. */
            idx_t j;
            for (j = 0; j < distinct_cnt; ++j) {
                if (distinct_arr[j].val == a_cur) break;
            }
            if (j < distinct_cnt) {
                /* Existing distinct value. */
                idx_t old_f = distinct_arr[j].freq;
                /* Update frequency counts. */
                freq_cnt_arr[old_f]--;
                distinct_arr[j].freq = old_f + 1;
                freq_cnt_arr[old_f + 1]++;   /* freq_cnt_arr has been allocated large enough */
            } else {
                /* New distinct value. */
                ensure_distinct_cap(distinct_cnt + 1);
                distinct_arr[distinct_cnt].val = a_cur;
                distinct_arr[distinct_cnt].freq = 1;
                freq_cnt_arr[1]++;          /* freq_cnt_arr has been allocated large enough */
                j = distinct_cnt;
                distinct_cnt++;
            }
            last_distinct_index = j;
        }
        computed_n = n;
    }
    return seq_arr[n - 1];
}
