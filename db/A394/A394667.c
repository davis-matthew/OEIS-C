/*
 * A394667  Integers which can be written as the sum of a prime and an anagram
 *          of that prime.
 * https://oeis.org/A394667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A394667: integers that can be written as the sum of a prime and an anagram of that prime */
/* The function builds the sequence lazily and caches already computed terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Comparison function for sorting seq_t values */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* ---------- Prime generation ---------- */
static uint64_t *primes = NULL;   /* list of primes found so far   */
static size_t    primes_cap = 0;
static size_t    primes_cnt = 0;
static uint64_t  last_checked = 1; /* last number tested for primality */

/* Append a new prime to the primes array */
static void add_prime(uint64_t p)
{
    if (primes_cnt == primes_cap) {
        primes_cap = (primes_cap ? primes_cap * 2 : 16);
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
        if (!primes) exit(1);
    }
    primes[primes_cnt++] = p;
}

/* Return the next prime ≥ last_checked */
static uint64_t next_prime(void)
{
    if (last_checked < 2) {
        last_checked = 2;
        add_prime(2);
        return 2;
    }
    uint64_t cand = last_checked;
    for (;;) {
        cand++;
        if (cand % 2 == 0) continue;
        bool prime = true;
        for (size_t i = 0; i < primes_cnt; i++) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime) {
            last_checked = cand;
            add_prime(cand);
            return cand;
        }
    }
}

/* ---------- Permutation generation for an anagram of a prime ---------- */
static void dfs_perm(uint64_t p, int pos, int len, seq_t curr,
                     int cnt[10], seq_t **out, size_t *len_out, size_t *cap_out)
{
    if (pos == len) {
        seq_t sum = (seq_t)p + curr;
        if (*len_out == *cap_out) {
            *cap_out = (*cap_out ? *cap_out * 2 : 16);
            *out = (seq_t *)realloc(*out, *cap_out * sizeof(seq_t));
            if (!*out) exit(1);
        }
        (*out)[(*len_out)++] = sum;
        return;
    }
    for (int d = 0; d < 10; d++) {
        if (!cnt[d]) continue;
        if (pos == 0 && d == 0) continue;   /* no leading zero */
        cnt[d]--;
        dfs_perm(p, pos + 1, len, curr * 10 + d, cnt, out, len_out, cap_out);
        cnt[d]++;
    }
}

/* Gather all sums p + anagram(p) for a single prime p */
static void gather_sums_from_prime(uint64_t p,
                                   seq_t **out, size_t *len, size_t *cap)
{
    int cnt[10] = {0};
    uint64_t tmp = p;
    int digits = 0;
    while (tmp) {
        cnt[tmp % 10]++;
        tmp /= 10;
        digits++;
    }
    if (digits == 0) return;   /* should not happen for primes ≥2 */
    dfs_perm(p, 0, digits, 0, cnt, out, len, cap);
}

/* ---------- Sequence storage ---------- */
static seq_t *terms = NULL;
static size_t terms_cnt = 0;

/* ---------- Main function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n ≤ 0 */

    static bool init_done = false;
    if (!init_done) {
        init_done = true;
        primes = NULL;
        primes_cap = primes_cnt = 0;
        last_checked = 1;
        terms = NULL;
        terms_cnt = 0;
    }

    /* Generate terms until we have at least n of them */
    while (terms_cnt < (size_t)n) {
        /* Process a batch of new primes */
        const size_t BATCH_SZ = 200;
        seq_t *new_sums = NULL;
        size_t new_len = 0, new_cap = 0;

        for (size_t i = 0; i < BATCH_SZ; i++) {
            uint64_t p = next_prime();
            gather_sums_from_prime(p, &new_sums, &new_len, &new_cap);
        }

        if (new_len == 0) break;   /* no more primes to process */

        /* Sort and deduplicate the newly generated sums */
        qsort(new_sums, new_len, sizeof(seq_t), cmp_seq_t);
        size_t uniq = 0;
        for (size_t i = 0; i < new_len; i++) {
            if (i == 0 || new_sums[i] != new_sums[uniq - 1]) {
                new_sums[uniq++] = new_sums[i];
            }
        }
        new_len = uniq;

        /* Merge the new sums with the existing sorted list of terms */
        size_t new_total = terms_cnt + new_len;
        seq_t *merged = (seq_t *)malloc(new_total * sizeof(seq_t));
        if (!merged) exit(1);

        size_t i = 0, j = 0, k = 0;
        while (i < terms_cnt && j < new_len) {
            if (terms[i] < new_sums[j]) {
                merged[k++] = terms[i++];
            } else if (new_sums[j] < terms[i]) {
                merged[k++] = new_sums[j++];
            } else {
                merged[k++] = terms[i++];
                j++;
            }
        }
        while (i < terms_cnt) merged[k++] = terms[i++];
        while (j < new_len)  merged[k++] = new_sums[j++];

        free(terms);
        terms = merged;
        terms_cnt = k;

        free(new_sums);
    }

    return terms[(size_t)n - 1];
}
