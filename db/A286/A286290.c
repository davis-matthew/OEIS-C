/*
 * A286290  A bisection of A064736.
 * https://oeis.org/A286290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 243, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A286290: a(n) is the n‑th term of the integer sequence
   produced by iteratively building A064736 as follows:
   Start with S = {1}. For m = 2,3,4,...:
   * if m is already in S, add (m‑1)*(m+1) to S;
   * else if neither m nor m+1 is in S, add m*(m+1) to S;
   * otherwise skip m.
   The resulting sorted list S is exactly the desired sequence. */

/* Note: This implementation memoises the values of A064736
   and returns the (n‑1)-th element (0‑based index). */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Binary search for seq_t value equivalent to integer v */
static bool seq_contains(const seq_t *arr, size_t len, idx_t v)
{
    seq_t tv = (seq_t)v;
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] < tv)
            lo = mid + 1;
        else if (arr[mid] > tv)
            hi = mid;
        else
            return true;
    }
    return false;
}

static void ensure_capacity(size_t *cap, seq_t **arr, size_t needed)
{
    if (*cap >= needed)
        return;
    size_t new_cap = *cap ? *cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

/* Static storage for the generated sequence S = A064736 */
static seq_t *a_seq = NULL;
static size_t a_len = 0;
static size_t a_cap = 0;
static idx_t next_m = 2;   /* next m to process */

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined; avoid UB */

    /* 0‑based target index in A064736 */
    idx_t target = n - 1;

    if (a_len == 0) {
        ensure_capacity(&a_cap, &a_seq, 1);
        a_seq[0] = (seq_t)1;
        a_len = 1;
        next_m = 2;
    }

    while (a_len <= (size_t)target) {
        idx_t m = next_m;
        bool in_m  = seq_contains(a_seq, a_len, m);
        bool in_m1 = seq_contains(a_seq, a_len, m + 1);
        seq_t new_val;

        if (in_m) {
            new_val = (seq_t)(m - 1) * ((seq_t)m + 1);
        } else if (!in_m && !in_m1) {
            new_val = (seq_t)m * ((seq_t)m + 1);
        } else {
            /* m is skipped */
            ++next_m;
            continue;
        }

        ensure_capacity(&a_cap, &a_seq, a_len + 1);
        a_seq[a_len++] = new_val;
        ++next_m;
    }

    return a_seq[target];
}
