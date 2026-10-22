/*
 * A329452  There are exactly two primes in {a(n+i) + a(n+j), 0 <= i < j <= 3}
 *          for any n: lexicographically earliest such sequence of distinct
 *          nonnegative integers.
 * https://oeis.org/A329452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lexicographically earliest sequence of distinct
 *            nonnegative integers such that every block of four consecutive
 *            terms has exactly two prime pairwise sums. This function
 *            generates the sequence iteratively, memoizing all already
 *            computed terms and numbers that have been used. For each new
 *            ter...
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>      /* for memset */
#include "../utils/is_prime.h"

/* static state */
static seq_t *seq_arr = NULL;      /* stores computed terms */
static size_t  seq_cap = 0;        /* allocated capacity of seq_arr   */
static size_t  seq_len = 0;        /* number of terms already computed */

static bool *used_arr = NULL;      /* bitmap of already used values   */
static size_t used_cap = 0;        /* allocated capacity of used_arr   */

/* enlarge seq_arr if needed so it can hold indexes up to "cap-1". */
static void ensure_seq_cap(size_t cap) {
    if (cap <= seq_cap) return;
    size_t newcap = seq_cap ? seq_cap : 64;
    while (newcap < cap) newcap <<= 1;
    seq_arr = (seq_t *)realloc(seq_arr, newcap * sizeof(seq_t));
    if (seq_arr == NULL) abort();
    seq_cap = newcap;
}

/* enlarge used_arr if needed so it can index values up to "idx". */
static void ensure_used_cap(size_t idx) {
    if (idx < used_cap) return;
    size_t newcap = used_cap ? used_cap : 64;
    while (newcap <= idx) newcap <<= 1;
    bool *tmp = (bool *)realloc(used_arr, newcap * sizeof(bool));
    if (tmp == NULL) abort();
    /* initialise the new part to false */
    memset(tmp + used_cap, 0, (newcap - used_cap) * sizeof(bool));
    used_arr = tmp;
    used_cap = newcap;
}

/* main sequence function */
seq_t a(idx_t n) {
    /* ensure capacity for nth term */
    if ((size_t)n >= seq_cap) ensure_seq_cap((size_t)n + 1);

    /* generate terms up to n */
    while (seq_len <= (size_t)n) {
        seq_t candidate = 0;
        while (1) {
            /* book‑keep the usage bitmap */
            size_t cand_index = (size_t)candidate;
            if (cand_index >= used_cap) ensure_used_cap(cand_index);
            if (used_arr[cand_index]) { /* already used */
                candidate++;
                continue;
            }

            bool ok = true;
            if (seq_len >= 3) {
                /* last three terms */
                seq_t a0 = seq_arr[seq_len - 3];
                seq_t a1 = seq_arr[seq_len - 2];
                seq_t a2 = seq_arr[seq_len - 1];

                /* count primes among the six pairwise sums */
                int prime_cnt = 0;
                seq_t s0 = a0 + a1;
                seq_t s1 = a0 + a2;
                seq_t s2 = a0 + candidate;
                seq_t s3 = a1 + a2;
                seq_t s4 = a1 + candidate;
                seq_t s5 = a2 + candidate;

                if (is_prime((idx_t)s0)) prime_cnt++;
                if (is_prime((idx_t)s1)) prime_cnt++;
                if (is_prime((idx_t)s2)) prime_cnt++;
                if (is_prime((idx_t)s3)) prime_cnt++;
                if (is_prime((idx_t)s4)) prime_cnt++;
                if (is_prime((idx_t)s5)) prime_cnt++;

                if (prime_cnt != 2) ok = false;
            }
            /* for seq_len < 3 the constraint is not applied */
            if (ok) {
                /* accept this candidate */
                seq_arr[seq_len] = candidate;
                used_arr[cand_index] = true;
                seq_len++;
                break;
            }
            candidate++;
        }
    }
    return seq_arr[(size_t)n];
}
