/*
 * A371462  Numbers such that the arithmetic mean of its digits is equal to
 *          the population standard deviation of its digits.
 * https://oeis.org/A371462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A371462. The sequence consists of all positive integers * whose
 *            digits have an arithmetic mean equal to the population standard
 *            * deviation of the digits. For a multiset of digits with counts
 *            c[d], * let k = Σ c[d], S = Σ d·c[d], Q = Σ d²·c[d]. The
 *            condition * mean = sd is equivalent to * * Q · k...
 * Range:     u8 n <= 10, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define POW10_SIZE 64          /* enough for all considered lengths */

static seq_t pow10[POW10_SIZE];
static bool pow10_inited = false;
static void init_pow10(void) {
    if (pow10_inited) return;
    pow10[0] = (seq_t)1;
    for (size_t i = 1; i < POW10_SIZE; ++i)
        pow10[i] = pow10[i-1] * (seq_t)10;
    pow10_inited = true;
}

/* ---------- dynamic array of terms ---------- */
static seq_t *term_arr = NULL;
static size_t term_len = 0, term_cap = 0;
static void bump_cap(void) {
    if (term_cap == 0) term_cap = 8;
    else term_cap <<= 1;
}
static void add_term(seq_t v) {
    if (term_len == term_cap) {
        bump_cap();
        term_arr = (seq_t *)realloc(term_arr, term_cap * sizeof(seq_t));
    }
    term_arr[term_len++] = v;
}

/* ---------- comparison for qsort ---------- */
static int cmp_seq(const void *a, const void *b) {
    const seq_t *va = (const seq_t *)a;
    const seq_t *vb = (const seq_t *)b;
    if (*va < *vb) return -1;
    if (*va > *vb) return  1;
    return 0;
}

/* ---------- permutation generator ---------- */
static void permute_digit_counts(int cnt[10], int depth, int k, seq_t cur) {
    if (depth == k) {
        add_term(cur);
        return;
    }
    for (int d = 0; d <= 9; ++d) {
        if (cnt[d] == 0) continue;
        if (depth == 0 && d == 0 && k > 1) continue;  /* no leading zero */
        cnt[d]--;
        permute_digit_counts(cnt, depth + 1, k, cur * (seq_t)10 + (seq_t)d);
        cnt[d]++;   /* backtrack */
    }
}

/* ---------- recursive enumeration of digit counts ---------- */
static void enumerate_counts(int digit, int remK, int remS, int remQ,
                             int cnt[10], int k) {
    if (digit == 10) {
        if (remK == 0 && remS == 0 && remQ == 0)
            permute_digit_counts(cnt, 0, k, (seq_t)0);
        return;
    }
    int maxC = remK;
    for (int c = 0; c <= maxC; ++c) {
        int newS = remS - digit * c;
        int newQ = remQ - digit * digit * c;
        if (newS < 0 || newQ < 0) break;
        /* crude pruning */
        if (newS > 9 * (remK - c)) continue;
        if (newQ > 81 * (remK - c)) continue;
        cnt[digit] = c;
        enumerate_counts(digit + 1, remK - c, newS, newQ, cnt, k);
        cnt[digit] = 0;   /* reset for next loop, not strictly needed */
    }
}

/* ---------- generate all valid numbers for a fixed length k ---------- */
static void generate_length(int k) {
    for (int S = 0; S <= 9 * k; ++S) {
        /* Check divisibility */
        int twoSsq = 2 * S * S;
        if (twoSsq % k != 0) continue;
        int Q_needed = twoSsq / k;
        /* run enumeration with these S, Q */
        int cnt[10] = {0};
        enumerate_counts(0, k, S, Q_needed, cnt, k);
    }
}

/* ---------- global state for memoisation ---------- */
static idx_t max_generated_k = 0;         /* maximum k processed so far */

/* ---------- ensure we have at least n terms ---------- */
static void ensure_terms(idx_t n) {
    init_pow10();
    /* grow until sufficient terms */
    while (term_len < n) {
        ++max_generated_k;
        generate_length((int)max_generated_k);
        /* sort and dedup */
        if (term_len == 0) continue;          /* should not happen */
        qsort(term_arr, term_len, sizeof(seq_t), cmp_seq);
        size_t write = 1;
        for (size_t i = 1; i < term_len; ++i) {
            if (term_arr[i] != term_arr[write-1]) {
                term_arr[write++] = term_arr[i];
            }
        }
        term_len = write;
        /* safety break to avoid infinite loop on pathological builds */
        if (max_generated_k > 200) break;
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined but safe */
    ensure_terms(n);
    /* at this point term_arr has at least n terms */
    return term_arr[n-1];
}
