/*
 * A395918  a(0) = 0, and then a(n) is the numbers of ways to write a(n-1) as
 *          a(i) * a(j) with i <= j < n.
 * https://oeis.org/A395918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs i ≤ j < n such that a(i)*a(j) = a(n‑1)
 *            a(0) = 0
 * Range:     u8 n <= 176, u16 n <= 2817, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *seq = NULL;            /* sequence array */
static size_t seq_cap = 0;           /* allocated capacity of seq */
static size_t seq_len = 0;           /* number of computed terms */

struct count_rec {
    seq_t val;      /* value in the sequence */
    size_t cnt;     /* number of occurrences in [0, seq_len-1] */
};

static struct count_rec *cnt = NULL;   /* array of distinct value counts */
static size_t cnt_cap = 0;             /* allocated capacity of cnt */
static size_t cnt_len = 0;             /* number of distinct values stored */

/* Ensure that seq has space for at least 'need' elements. */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1U : 16;
    while (new_cap < need) new_cap <<= 1U;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure that cnt has space for at least 'need' records. */
static void ensure_cnt_cap(size_t need)
{
    if (cnt_cap >= need) return;
    size_t new_cap = cnt_cap ? cnt_cap << 1U : 8;
    while (new_cap < need) new_cap <<= 1U;
    struct count_rec *tmp = (struct count_rec *)realloc(cnt,
                                 new_cap * sizeof(struct count_rec));
    if (!tmp) abort();
    cnt = tmp;
    cnt_cap = new_cap;
}

/* Return the occurrence count of 'val' among indices < seq_len. */
static size_t get_count(seq_t val)
{
    for (size_t i = 0; i < cnt_len; ++i)
        if (cnt[i].val == val) return cnt[i].cnt;
    return 0;
}

/* Increment the occurrence count of 'val' by one. */
static void increment_count(seq_t val)
{
    for (size_t i = 0; i < cnt_len; ++i)
        if (cnt[i].val == val) {
            ++cnt[i].cnt;
            return;
        }
    /* new distinct value */
    ensure_cnt_cap(cnt_len + 1);
    cnt[cnt_len].val = val;
    cnt[cnt_len].cnt = 1;
    ++cnt_len;
}

/* Initialise with a(0)=0 if not already done. */
static void init_sequence(void)
{
    if (seq_len > 0) return;         /* already initialised */
    ensure_seq_cap(16);
    seq[0] = (seq_t)0;
    seq_len = 1;
    ensure_cnt_cap(8);
    cnt[0].val  = (seq_t)0;
    cnt[0].cnt  = 1;
    cnt_len = 1;
}

/* Main entry point required by the problem statement. */
seq_t a(idx_t n)
{
    init_sequence();   /* ensure that a(0) is present */

    /* Compute terms up to n if required. */
    while (seq_len <= (size_t)n) {
        size_t m = seq_len;                 /* index to be computed */
        seq_t prev = seq[m - 1];            /* a(m-1) */
        size_t res = 0;                     /* accumulator for the pair count */

        if (prev == (seq_t)0) {
            /* a(m-1) == 0: count pairs involving at least one zero value. */
            size_t k = get_count((seq_t)0);          /* number of zeros seen */
            size_t m_tot = m;                       /* total previous indices */
            /* Pairs (zero,zero) */
            res += k * (k + 1) / 2;
            /* Pairs (zero,nonzero) */
            res += k * (m_tot - k);
        } else {
            /* Enumerate divisors of prev. */
            for (idx_t d = 1; (seq_t)d * (seq_t)d <= prev; ++d) {
                if (prev % (seq_t)d != 0) continue;
                seq_t u = (seq_t)d;
                seq_t w = prev / u;
                if (u > w) continue;   /* avoid double counting */
                size_t cnt_u = get_count(u);
                size_t cnt_w = get_count(w);
                if (cnt_u == 0 || cnt_w == 0) continue;

                if (u == w) {
                    /* pairs (i,i) and distinct pairs with the same value */
                    res += cnt_u * (cnt_u + 1) / 2;
                } else {
                    res += cnt_u * cnt_w;
                }
            }
        }

        /* Store the computed value and update the counts. */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = (seq_t)res;
        ++seq_len;
        increment_count((seq_t)res);
    }

    return seq[n];
}
