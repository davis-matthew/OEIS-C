/*
 * A394504  Numbers such that each digit d_i is equal to the number of digits
 *          to its right that are strictly less than d_i.
 * https://oeis.org/A394504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the (n‑th smallest) decimal number whose digits d_i
 *            satisfy d_i = #{j > i : d_j < d_i}. Computed by exhaustive
 *            generation of all such numbers (length ≤10) and sorting unique
 *            values
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build.  No other types are used. */
static int initialized = 0;
static seq_t *values = NULL;
static size_t value_count = 0;

/* Simple dynamic array for seq_t values. */
typedef struct {
    seq_t *data;
    size_t size;
    size_t cap;
} Vec;

static void vec_init(Vec *v, size_t cap) {
    v->cap = cap;
    v->size = 0;
    v->data = (seq_t *)malloc(cap * sizeof(seq_t));
}

static void vec_push(Vec *v, seq_t val) {
    if (v->size == v->cap) {
        v->cap = (v->cap == 0) ? 64 : v->cap * 2;
        v->data = (seq_t *)realloc(v->data, v->cap * sizeof(seq_t));
    }
    v->data[v->size++] = val;
}

/* Compare function for qsort. */
static int cmp_seq(const void *a, const void *b) {
    if (*(seq_t *)a < *(seq_t *)b) return -1;
    if (*(seq_t *)a > *(seq_t *)b) return 1;
    return 0;
}

/* Recursive construction of all valid numbers of a given length.  The array
   'digits' holds the number being built.  The array 'freq' contains the
   frequency of each decimal digit in the suffix already fixed to the right. */
static void build_seq(int len, int pos, int freq[10], int digits[10], Vec *out) {
    if (pos < 0) {
        /* All digits set; compute numeric value. */
        seq_t val = 0;
        for (int i = 0; i < len; ++i) {
            val = val * 10 + (seq_t)digits[i];
        }
        vec_push(out, val);
        return;
    }

    for (int d = 0; d <= 9; ++d) {
        /* Count how many digits in the suffix are strictly less than d. */
        int less = 0;
        for (int k = 0; k < d; ++k) less += freq[k];
        if (less == d) {
            digits[pos] = d;
            freq[d]++;
            build_seq(len, pos - 1, freq, digits, out);
            freq[d]--;
        }
    }
}

/* Generate all sequence values, store sorted unique ascending list.  This is
   done lazily at first call of a(). */
static void init_sequence(void) {
    Vec tmp;
    vec_init(&tmp, 0);

    int freq[10] = {0};
    int digits[10];

    /* Length 1 */
    digits[0] = 0;
    freq[0] = 1;
    build_seq(1, -1, freq, digits, &tmp);
    freq[0] = 0;

    for (int len = 2; len <= 10; ++len) {
        freq[0] = 1;          /* last digit is 0 */
        digits[len - 1] = 0;
        build_seq(len, len - 2, freq, digits, &tmp);
        freq[0] = 0;
    }

    /* Sort and remove duplicates. */
    if (tmp.size == 0) {
        values = NULL;
        value_count = 0;
        free(tmp.data);
        return;
    }
    qsort(tmp.data, tmp.size, sizeof(seq_t), cmp_seq);

    size_t uniq = 1;
    for (size_t i = 1; i < tmp.size; ++i) {
        if (tmp.data[i] != tmp.data[uniq - 1]) {
            tmp.data[uniq++] = tmp.data[i];
        }
    }

    values = tmp.data;
    value_count = uniq;
    initialized = 1;
}

/* Public function required by the build. */
seq_t a(idx_t n) {
    if (!initialized) {
        init_sequence();
    }
    if (n == 0 || n > value_count) {
        return (seq_t)0;
    }
    return values[n - 1];
}
