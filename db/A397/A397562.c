/*
 * A397562  Lexicographically earliest sequence of distinct positive integers
 *          such that for any prime number p, the absolute difference of the
 *          p-adic valuations of two consecutive terms is at most 1.
 * https://oeis.org/A397562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 247, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest sequence A397562.
   a(1) = 1.  For n > 1 choose the smallest integer x that has not appeared
   before and satisfies |v_p(x) - v_p(a(n‑1))| ≤ 1 for every prime p
   (equivalently, the ratio a(n)/a(n‑1) has square‑free numerator and
   denominator).  The sequence is constructed lazily and memoised. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PRIME_LIMIT 2000000  /* sufficient to factor 64‑bit integers */

/* ---------- prime table ---------- */
static uint32_t *prime_table = NULL;
static size_t prime_count = 0;

static void build_prime_table(void)
{
    if (prime_count) return;  /* already built */

    bool *is_composite = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    size_t est = PRIME_LIMIT / 10 + 10;          /* rough estimate */
    prime_table           = (uint32_t*)malloc(est * sizeof(uint32_t));
    prime_count = 0;

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_count == est) {          /* grow array if needed */
                est <<= 1;
                prime_table = (uint32_t*)realloc(prime_table, est * sizeof(uint32_t));
            }
            prime_table[prime_count++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
}

/* ---------- factorisation of a 64‑bit integer ---------- */
static void factor_uint64(uint64_t n,
                          uint32_t *pr, int *exp,
                          size_t *cnt)
{
    size_t k = 0;
    uint64_t m = n;

    for (size_t i = 0; i < prime_count &&
             (uint64_t)prime_table[i] * prime_table[i] <= m; ++i) {
        uint32_t p = prime_table[i];
        if (m % p == 0) {
            int e = 0;
            do {
                m /= p;
                ++e;
            } while (m % p == 0);
            pr[k]   = p;
            exp[k]  = e;
            ++k;
        }
    }
    if (m > 1) { /* remaining prime factor */
        pr[k]   = (uint32_t)m;
        exp[k]  = 1;
        ++k;
    }
    *cnt = k;
}

/* ---------- test of the |difference| ≤ 1 condition ---------- */
static bool valid_transition(const uint32_t *pr_cur, const int *exp_cur,
                             size_t cur_cnt,
                             const uint32_t *pr_can, const int *exp_can,
                             size_t can_cnt)
{
    size_t i = 0, j = 0;
    while (i < cur_cnt || j < can_cnt) {
        uint32_t p;
        int e_cur = 0, e_can = 0;
        if (i < cur_cnt && (j >= can_cnt || pr_cur[i] < pr_can[j])) {
            p = pr_cur[i];
            e_cur = exp_cur[i];
            e_can = 0;
            ++i;
        } else if (j < can_cnt && (i >= cur_cnt || pr_can[j] < pr_cur[i])) {
            p = pr_can[j];
            e_cur = 0;
            e_can = exp_can[j];
            ++j;
        } else { /* equal primes */
            p = pr_cur[i];
            e_cur = exp_cur[i];
            e_can = exp_can[j];
            ++i; ++j;
        }
        if (abs(e_cur - e_can) > 1) return false;
    }
    return true;
}

/* ---------- dynamic array of previously used integer values ---------- */
static uint8_t *used = NULL;
static size_t used_cap = 0;

static void ensure_used_cap(size_t n)
{
    if (n <= used_cap) return;
    size_t new_cap = used_cap ? (used_cap << 1) : 32;
    if (new_cap < n) new_cap = n;
    uint8_t *p = (uint8_t*)realloc(used, new_cap);
    if (!p) abort();
    memset(p + used_cap, 0, new_cap - used_cap);
    used = p;
    used_cap = new_cap;
}

/* ---------- dynamic array of computed sequence values ---------- */
static seq_t *seq = NULL;
static size_t seq_cap = 0;   /* number of allocated slots (seq[0] unused) */
static size_t seq_len = 0;   /* highest index computed so far */

static void ensure_seq_cap(size_t n)
{
    if (n <= seq_cap) return;
    size_t new_cap = seq_cap ? (seq_cap << 1) : 16;
    if (new_cap < n) new_cap = n;
    seq_t *p = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    if (!p) abort();
    seq = p;
    seq_cap = new_cap;
}

/* ---------- generate terms up to index n (1‑based) ---------- */
static void generate_up_to(idx_t n)
{
    while (seq_len < (size_t)n) {
        /* factorise current term */
        seq_t cur = seq[seq_len];
        uint64_t cur64 = (uint64_t)cur;
        uint32_t cur_pr[64], can_pr[64];
        int cur_exp[64], can_exp[64];
        size_t cur_cnt, can_cnt;

        factor_uint64(cur64, cur_pr, cur_exp, &cur_cnt);

        /* search for next term */
        seq_t next_value = 0;
        for (size_t cand = 1; ; ++cand) {
            if (cand < used_cap && used[cand]) continue;

            if (cand > SIZE_MAX) break; /* safety, unlikely to happen */

            uint64_t cand64 = (uint64_t)cand;
            factor_uint64(cand64, can_pr, can_exp, &can_cnt);
            if (valid_transition(cur_pr, cur_exp, cur_cnt,
                                 can_pr, can_exp, can_cnt)) {
                next_value = (seq_t)cand;
                break;
            }
            /* Ensure used array can hold this index before next loop iteration */
            ensure_used_cap(cand + 1);
        }

        /* store the new term */
        ++seq_len;
        ensure_seq_cap(seq_len + 1);
        seq[seq_len] = next_value;

        /* mark used */
        ensure_used_cap((size_t)next_value + 1);
        used[(size_t)next_value] = 1;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    static bool init = false;
    if (!init) {
        init = true;
        ensure_seq_cap(2);
        seq[1] = (seq_t)1;
        seq_len = 1;
        ensure_used_cap(2);
        used[1] = 1;
        build_prime_table();
    }
    if (n == 0) return (seq_t)0;  /* undefined but safe */
    generate_up_to(n);
    return seq[n];
}
