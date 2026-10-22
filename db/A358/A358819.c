/*
 * A358819  Numbers k such that for some r we have w(1) + ... + w(k - 1) = w(k
 *          + 1) + ... + w(k + r), where w(i) = A000120(i).
 * https://oeis.org/A358819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers k such that ∃r≥1 with ∑_{i=1}^{k-1} popcount(i) =
 *            ∑_{i=k+1}^{k+r} popcount(i) (w(i) = A000120(i))
 * Range:     u8 n <= 2, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----- auxiliary struct for pending targets ----- */
struct PendEntry {
    seq_t target;
    idx_t k;
};

/* ----- global state for cumulative sums, terms and pending entries ----- */
static seq_t *S = NULL;          /* cumulative popcount array, S[0] = 0      */
static idx_t S_len = 0;           /* largest index stored in S              */

static seq_t *terms = NULL;      /* collected terms of the sequence         */
static size_t terms_cap = 0;
static size_t terms_cnt = 0;

static struct PendEntry *pend = NULL; /* pending (target,k) pairs          */
static size_t pend_cap = 0;
static size_t pend_len = 0;

static idx_t last_k_processed = 0; /* largest k that has been examined    */

/* ----- helper: grow S to accommodate up to new_len (inclusive) ----- */
static void grow_S(idx_t new_len)
{
    if (new_len <= S_len) return;
    seq_t *tmp = (seq_t *)realloc(S, (new_len + 1) * sizeof(seq_t));
    if (!tmp) abort();
    S = tmp;
    for (idx_t i = S_len + 1; i <= new_len; ++i) {
        unsigned w = __builtin_popcountll((unsigned long long)i);
        S[i] = S[i - 1] + (seq_t)w;
    }
    S_len = new_len;
}

/* ----- helper: ensure capacity for terms array ----- */
static void ensure_terms_cap(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* ----- helper: ensure capacity for pending array ----- */
static void ensure_pend_cap(size_t needed)
{
    if (needed <= pend_cap) return;
    size_t new_cap = pend_cap ? pend_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    struct PendEntry *tmp = (struct PendEntry *)realloc(pend,
                             new_cap * sizeof(*tmp));
    if (!tmp) abort();
    pend = tmp;
    pend_cap = new_cap;
}

/* ----- binary search in S over [lo,hi] for target value ----- */
static int binary_search_S(idx_t lo, idx_t hi, seq_t target)
{
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t v = S[mid];
        if (v == target) return (int)mid;
        if (v < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

/* ----- process new k values up to new_limit ----- */
static void process_new_ks(idx_t new_limit)
{
    for (idx_t k = last_k_processed + 1; k <= new_limit; ++k) {
        unsigned w_k = __builtin_popcountll((unsigned long long)k);
        seq_t target = S[k - 1] + S[k];   /* S(k-1)+S(k) */
        if (target > S[new_limit]) {
            /* cannot be found yet; postpone */
            ensure_pend_cap(pend_len + 1);
            pend[pend_len].target = target;
            pend[pend_len].k = k;
            ++pend_len;
        } else {
            if (binary_search_S(k + 1, new_limit, target) >= 0) {
                ensure_terms_cap(terms_cnt + 1);
                terms[terms_cnt++] = (seq_t)k;
            }
        }
    }
    last_k_processed = new_limit;
}

/* ----- re‑evaluate pending targets after S has grown ----- */
static void recheck_pending(idx_t new_limit)
{
    size_t i = 0;
    while (i < pend_len) {
        seq_t target = pend[i].target;
        idx_t k = pend[i].k;
        if (target <= S[new_limit]) {
            if (binary_search_S(k + 1, new_limit, target) >= 0) {
                ensure_terms_cap(terms_cnt + 1);
                terms[terms_cnt++] = (seq_t)k;
                /* remove this pending entry by swapping with last */
                pend[i] = pend[--pend_len];
                continue;   /* re‑check swapped element */
            }
        }
        ++i;
    }
}

/* ----- generate terms until we have at least need_cnt terms ----- */
static void generate_upto_terms(size_t need_cnt)
{
    if (terms_cnt >= need_cnt) return;

    if (!S) {                   /* first time: allocate base array */
        S_len = 0;
        S = (seq_t *)malloc(sizeof(seq_t));
        if (!S) abort();
        S[0] = 0;
    }

    idx_t limit = S_len > 0 ? S_len : 1000;
    while (terms_cnt < need_cnt) {
        idx_t new_limit = limit * 2;
        if (new_limit == 0) new_limit = 1000;   /* safety */
        grow_S(new_limit);
        process_new_ks(new_limit);
        recheck_pending(new_limit);
        limit = new_limit;
    }
}

/* ----- public function required by the evaluation system ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1: guard against 0   */
    generate_upto_terms((size_t)n);
    return terms[n - 1];
}
