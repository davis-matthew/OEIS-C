/*
 * A350511  a(n) is the unique k such that A124145(n) is the sum of k
 *          consecutive terms among the first n-1 terms of A124145.
 * https://oeis.org/A350511
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k where A124145(n) = A124145(j+1) + ... + A124145(j+k)
 *            for the unique window within the first n-1 terms; computed by
 *            generating A124145 (a(1)=1, a(2)=2, then smallest larger number
 *            that is a sum of consecutive earlier terms in exactly one way)
 *            and a two-pointer window search
 * Range:     u8 n <= 56, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 100 terms through n = 9708 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static inline int A124145_count_exact(const seq_t *arr, idx_t len, seq_t target)
{
    int cnt = 0;
    seq_t sum = 0;
    idx_t left = 1;
    for (idx_t right = 1; right <= len; right++) {
        sum += arr[right];
        while (sum > target && left <= right) {
            sum -= arr[left];
            left++;
        }
        if (sum == target) {
            cnt++;
            if (cnt > 1)
                return cnt;
        }
    }
    return cnt;
}

static inline void A124145_grow(idx_t *cap, seq_t **seq, idx_t needed)
{
    if (needed <= *cap)
        return;
    idx_t new_cap = *cap ? *cap << 1 : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    *seq = mem_resize(seq_t, *seq, new_cap);
    *cap = new_cap;
}

static seq_t A124145_a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t len = 0;
    static idx_t cap = 0;

    if (len == 0) {
        A124145_grow(&cap, &seq, 4);
        seq[1] = 1;
        seq[2] = 2;
        len = 2;
    }
    if (n <= len)
        return seq[n];
    while (len < n) {
        seq_t candidate = seq[len] + 1;
        while (1) {
            int cnt = A124145_count_exact(seq, len, candidate);
            if (cnt == 1) {
                len++;
                A124145_grow(&cap, &seq, len + 1);
                seq[len] = candidate;
                break;
            }
            candidate++;
        }
    }
    return seq[n];
}

seq_t a(idx_t n)
{
    seq_t target = A124145_a(n);
    seq_t sum = 0;
    idx_t left = 1;
    for (idx_t right = 1; right + 1 <= n; right++) {
        sum += A124145_a(right);
        while (sum > target && left <= right) {
            sum -= A124145_a(left);
            left++;
        }
        if (sum == target)
            return (seq_t)(right - left + 1);
    }
    return 0;
}
