/*
 * A259430  a(n) is the arithmetic mean of the first n terms in A073883.
 * https://oeis.org/A259430
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{k=1..n} A073883(k); computed by search
 *            (A073883 greedy construction)
 * Range:     u8 n <= 13, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 75 (2026-11-07)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static bool A259430_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool A259430_is_prime_big(seq_t n) {
    if (n < 2) return false;
    if (A259430_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t b = (seq_t)base_list[i];
        if (b >= n) continue;

        seq_t x = 1;
        seq_t pw = b % n;
        seq_t ex = d;
        while (ex > 0) {
            if (ex & 1) x = (x * pw) % n;
            pw = (pw * pw) % n;
            ex >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *A259430_seq = NULL;
static idx_t A259430_seq_cap = 0;
static idx_t A259430_seq_len = 0;

static unsigned char *A259430_used = NULL;
static idx_t A259430_used_cap = 0;

static seq_t A259430_sum = 0;

static void A259430_ensure_seq_cap(idx_t size)
{
    if (A259430_seq_cap >= size)
        return;
    size_t newcap = A259430_seq_cap ? A259430_seq_cap * 2 : 8;
    if (newcap < size)
        newcap = size;
    A259430_seq = mem_resize(seq_t, A259430_seq, newcap);
    A259430_seq_cap = newcap;
}

static void A259430_ensure_used_cap(idx_t idx)
{
    if (A259430_used_cap > idx)
        return;
    size_t newcap = A259430_used_cap ? A259430_used_cap * 2 : 8;
    while (newcap <= (size_t)idx)
        newcap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(A259430_used, newcap);
    memset(tmp + A259430_used_cap, 0, newcap - A259430_used_cap);
    A259430_used = tmp;
    A259430_used_cap = newcap;
}

static seq_t A073883_a(idx_t n)
{
    if (A259430_seq_len == 0) {
        A259430_ensure_seq_cap(2);
        A259430_seq[1] = 1;
        A259430_seq_len = 1;
        A259430_sum = 1;
        A259430_ensure_used_cap(1);
        A259430_used[1] = 1;
    }

    while (A259430_seq_len < n) {
        idx_t cur = A259430_seq_len + 1;
        seq_t prev_sum = A259430_sum;
        seq_t cand = 1;

        while (1) {
            A259430_ensure_used_cap((idx_t)cand);
            if (A259430_used[(size_t)cand])
                { ++cand; continue; }

            seq_t new_sum = prev_sum + cand;
            if (new_sum % (seq_t)cur == 0) {
                seq_t p = new_sum / (seq_t)cur;
                if (A259430_is_prime_big(p)) {
                    A259430_ensure_seq_cap(cur + 1);
                    A259430_seq[cur] = cand;
                    A259430_sum = new_sum;
                    A259430_used[(size_t)cand] = 1;
                    break;
                }
            }
            ++cand;
        }

        A259430_seq_len = cur;
    }

    return A259430_seq[n];
}

seq_t a(idx_t n)
{
    A073883_a(n);
    seq_t s = 0;
    for (idx_t i = 1; i <= n; i++)
        s += A259430_seq[i];
    return s / (seq_t)n;
}
