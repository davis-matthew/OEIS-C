/*
 * A351328  a(n) is equal to the sum of the factorials of the digits of
 *          a(n-1), with a(1) = 0; each time a duplicated term appears, we
 *          replace it with the smallest integer not yet in the sequence and
 *          iterate.
 * https://oeis.org/A351328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 0. For n > 1, let s be the sum of factorials of the
 *            decimal digits of a(n−1). If s has not yet appeared in the
 *            sequence, then a(n) = s; otherwise a(n) is the smallest
 *            non‑negative integer not yet present. This implementation
 *            memoises the sequence values and keeps a bitmap of the
 *            already‑us...
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* factorials of digits 0..9, precomputed once */
static const seq_t fact_digit[10] = {
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)24,
    (seq_t)120, (seq_t)720, (seq_t)5040, (seq_t)40320, (seq_t)362880
};

/* Sequence values  a(1), a(2), ... */
static seq_t *seq_vals = NULL;
static size_t seq_cap = 0;   /* capacity of seq_vals */
static size_t seq_len = 0;   /* number of stored values */

/* Bitmap of integers that have appeared (up to UINT64_MAX) */
static unsigned char *used_flag = NULL;
static size_t used_cap = 0;   /* capacity of used_flag */

/* Smallest non‑negative integer not yet used */
static uint64_t smallest_unused = 0;

/* Ensure seq_vals has capacity at least 'need'. */
static void ensure_seq_cap(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? (seq_cap << 1) : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Ensure used_flag has capacity at least 'need' (need is a count, not an index). */
static void ensure_used_cap(size_t need)
{
    if (need <= used_cap) return;
    size_t new_cap = used_cap ? (used_cap << 1) : 16;
    while (new_cap <= need) new_cap <<= 1;
    unsigned char *new_flag = (unsigned char *)realloc(used_flag,
                                                       new_cap * sizeof(unsigned char));
    /* zero the newly allocated part */
    memset(new_flag + used_cap, 0, (new_cap - used_cap) * sizeof(unsigned char));
    used_flag = new_flag;
    used_cap = new_cap;
}

/* Initialise data structures once. */
static void init_sequence(void)
{
    if (seq_vals != NULL) return;  /* already initialised */

    /* first term a(1) = 0 */
    ensure_seq_cap(16);
    seq_vals[0] = (seq_t)0;
    seq_len = 1;

    /* bitmap for used integers */
    ensure_used_cap(1);
    used_flag[0] = 1;
    smallest_unused = 1;
}

/* Sum of factorials of decimal digits of x */
static seq_t digit_factorial_sum(seq_t x)
{
    if (x == (seq_t)0) return fact_digit[0];
    seq_t sum = 0;
    while (x != (seq_t)0) {
        int d = (int)(x % (seq_t)10);
        sum += fact_digit[d];
        x /= (seq_t)10;
    }
    return sum;
}

/* Build the sequence up to index n (1‑based). */
static void compute_to(idx_t n)
{
    init_sequence();
    while ((idx_t)seq_len < n) {
        seq_t prev = seq_vals[seq_len - 1];
        seq_t cand = digit_factorial_sum(prev);

        bool used = false;
        if (cand <= (seq_t)UINT64_MAX) {
            size_t idx = (size_t)cand;
            if (idx < used_cap && used_flag[idx]) used = true;
        }

        if (used) cand = (seq_t)smallest_unused;

        /* Record cand in bitmap */
        if (cand <= (seq_t)UINT64_MAX) {
            size_t idx = (size_t)cand;
            ensure_used_cap(idx + 1);
            used_flag[idx] = 1;
        }

        /* Update smallest_unused if needed */
        if (cand == (seq_t)smallest_unused) {
            do {
                ++smallest_unused;
            } while ((size_t)smallest_unused < used_cap && used_flag[(size_t)smallest_unused]);

            /* Ensure the bitmap can be indexed by smallest_unused */
            ensure_used_cap((size_t)smallest_unused + 1);
        }

        /* Append to sequence */
        ensure_seq_cap(seq_len + 1);
        seq_vals[seq_len] = cand;
        ++seq_len;
    }
}

/* Public function required by the tests. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n=0, return 0 */
    compute_to(n);
    return seq_vals[n - 1];
}
