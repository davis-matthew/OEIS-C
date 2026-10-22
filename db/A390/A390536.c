/*
 * A390536  Composite integers k = b * c such that A029837(k) < A029837(b) +
 *          A029837(c) for all b>1, c>1, where A029837(x) = ceiling(log_2(x)).
 * https://oeis.org/A390536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------- OEIS A390536 --------------------
   Sequence: Composite integers k such that for all
   b>1, c>1 with k = b*c we have
      ceil(log2(k)) < ceil(log2(b)) + ceil(log2(c)).
   The function returns the n-th such integer, with a(1)=15.
   --------------------------------------------------- */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --------- small helpers ------------------------------------------ */
static inline int ceil_log2_uint64(uint64_t v)
{
    if (v <= 1) return 0;
    /* highest set bit position of v-1 */
    int l = 63 - __builtin_clzll(v - 1);
    return l + 1;
}
static inline int ceil_log(uint64_t x)
{
    return ceil_log2_uint64(x);
}

/* --------- sieve for smallest prime factors ----------------------- */
static uint32_t *spf = NULL;
static size_t spf_size = 0;

static void build_spf(size_t limit)
{
    if (limit <= spf_size) return;
    uint32_t *new_spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    if (!new_spf) exit(1);
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = i;
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    if (!new_spf[j]) new_spf[j] = i;
            }
        }
    }
    free(spf);
    spf = new_spf;
    spf_size = limit;
}

/* --------- factorisation ------------------------------------------- */
static void factorize(uint64_t n, uint32_t *pr, int *exp)
{
    size_t cnt = 0;
    while (n > 1) {
        uint32_t p = spf[(uint32_t)n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            e++;
        }
        pr[cnt] = p;
        exp[cnt] = e;
        cnt++;
    }
    pr[cnt] = 0;               /* sentinel */
}

/* --------- divisor test --------------------------------------------- */
static bool check_divisor_rec(size_t idx,
                              uint64_t curr,
                              const uint32_t *pr,
                              const int *exp,
                              int exp_cnt,
                              uint64_t k,
                              int ceil_k)
{
    if (idx == exp_cnt) {
        if (curr <= 1 || curr >= k)   /* skip 1 and k itself */
            return true;
        uint64_t b = curr;
        uint64_t c = k / b;
        if (ceil_k >= ceil_log(b) + ceil_log(c))
            return false;             /* bad factor pair found */
        return true;
    } else {
        uint64_t d = curr;
        for (int e = 0; e <= exp[idx]; ++e) {
            if (!check_divisor_rec(idx + 1, d, pr, exp, exp_cnt, k, ceil_k))
                return false;
            d *= pr[idx];
        }
        return true;
    }
}

static bool good_k(uint64_t k)
{
    if (k < 4) return false;                     /* must be composite */
    if (k > spf_size) build_spf(k);
    if (spf[(uint32_t)k] == (uint32_t)k) return false;   /* prime */

    int ceil_k = ceil_log(k);

    uint32_t pr[32];
    int exp[32];
    factorize(k, pr, exp);

    int exp_cnt = 0;
    while (pr[exp_cnt]) exp_cnt++;

    return check_divisor_rec(0, 1ULL, pr, exp, exp_cnt, k, ceil_k);
}

/* --------- storage for found terms --------------------------------- */
static seq_t *term_arr = NULL;
static size_t term_count = 0;
static size_t term_cap   = 0;

static void push_term(seq_t val)
{
    if (term_count == term_cap) {
        term_cap = term_cap ? term_cap * 2 : 128;
        seq_t *tmp = (seq_t*)realloc(term_arr, term_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        term_arr = tmp;
    }
    term_arr[term_count++] = val;
}

/* --------- main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n=0, but harmless */

    static uint64_t cur_k = 4;

    while (term_count < n) {
        if (cur_k > spf_size)
            build_spf(cur_k + 1024);          /* extend SPF table */
        if (good_k(cur_k))
            push_term((seq_t)cur_k);
        cur_k++;
    }
    return term_arr[n - 1];
}
