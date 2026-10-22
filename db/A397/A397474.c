/*
 * A397474  a(1) = 0. For n > 1, a(n) is the number of ways to write a(n-1) as
 *          the product of two previous terms with distinct indices.
 * https://oeis.org/A397474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1, a(n) is the number of unordered pairs of
 *            distinct indices i<j<n such that a(i)*a(j)=a(n-1)
 * Range:     u8 n <= 94, u16 n <= 2612, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq = NULL;          /* computed terms, 0-based index */
static size_t seq_cap = 0;
static size_t seq_len = 0;

static seq_t *vals = NULL;          /* distinct values encountered so far   */
static size_t *freq = NULL;         /* frequency of each distinct value     */
static size_t vals_cap = 0;
static size_t vals_len = 0;
static size_t zero_idx = SIZE_MAX;  /* index in vals[] where value==0, if any */

static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    seq = tmp;
    seq_cap = new_cap;
}

static void ensure_vals_capacity()
{
    if (vals_len == vals_cap) {
        size_t new_cap = (vals_cap == 0) ? 16 : vals_cap * 2;
        vals = (seq_t*)realloc(vals, new_cap * sizeof(seq_t));
        freq = (size_t*)realloc(freq, new_cap * sizeof(size_t));
        vals_cap = new_cap;
    }
}

static void add_value(seq_t v)
{
    /* update frequency tables */
    for (size_t i = 0; i < vals_len; ++i) {
        if (vals[i] == v) {
            ++freq[i];
            return;
        }
    }
    /* new distinct value */
    ensure_vals_capacity();
    vals[vals_len] = v;
    freq[vals_len] = 1;
    if (v == 0) zero_idx = vals_len;
    ++vals_len;
}

static size_t find_index(seq_t v)
{
    for (size_t i = 0; i < vals_len; ++i) {
        if (vals[i] == v) return i;
    }
    return SIZE_MAX; /* not found */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard */

    /* ensure seq initialized with a(1)=0 */
    if (seq_len == 0) {
        ensure_seq_capacity(1);
        seq[0] = 0;
        seq_len = 1;
        /* init value tables */
        ensure_vals_capacity();
        vals[0] = 0;
        freq[0] = 1;
        zero_idx = 0;
        vals_len = 1;
    }

    /* extend as needed */
    for (idx_t cur = seq_len+1; cur <= n; ++cur) {
        /* target is a(cur-1) */
        seq_t target = seq[cur-2];
        size_t total_indices = cur-1;          /* number of previous indices */
        seq_t count = 0;

        /* number of zeros currently */
        size_t zcnt = (zero_idx != SIZE_MAX) ? freq[zero_idx] : 0;

        /* handle zero target specially (avoids dividing by zero) */
        if (target == 0) {
            seq_t z = (seq_t)zcnt;
            seq_t total = (seq_t)total_indices;
            count = z * (total - z) + z * (z - 1) / 2;
        } else {
            /* pairs not involving zero */
            for (size_t i = 0; i < vals_len; ++i) {
                seq_t d = vals[i];
                if (d == 0) continue;          /* zero cannot multiply to non‑zero target */
                if (target % d != 0) continue; /* not a divisor */

                seq_t e = target / d;
                size_t j = find_index(e);
                if (j == SIZE_MAX) continue;   /* e not present */
                if (i < j) {
                    count += (seq_t)freq[i] * (seq_t)freq[j];
                } else if (i == j) {
                    seq_t f = (seq_t)freq[i];
                    count += f * (f - 1) / 2;
                }
                /* i > j will be handled when j visit i */
            }
        }

        /* store computed term and update tables */
        ensure_seq_capacity(cur);
        seq[cur-1] = count;
        ++seq_len;
        add_value(count);
    }

    return seq[n-1];
}
