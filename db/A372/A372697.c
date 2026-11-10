/*
 * A372697  Index k such that A280866(k) = A019565(n) or 0 if A019565(n) does
 *          not appear in A280866.
 * https://oeis.org/A372697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A280866(k) = A019565(n), where A019565(n) =
 *            Product_{bit i set in n} prime(i+1); found by running the greedy
 *            generator of A280866 until the value appears
 * Range:     u8 n <= 14, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 57 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static void A280866_get_prime_factors(seq_t n, uint32_t *arr, size_t *cnt)
{
    *cnt = 0;
    seq_t tmp = n;
    if (tmp <= 1) return;
    while ((tmp & 1) == 0) {
        if (*cnt == 0 || arr[0] != 2) arr[(*cnt)++] = 2;
        tmp >>= 1;
    }
    for (uint32_t f = 3; (seq_t)f * (seq_t)f <= tmp; f += 2) {
        if (tmp % f == 0) {
            arr[(*cnt)++] = f;
            while (tmp % f == 0) tmp /= f;
        }
    }
    if (tmp > 1) arr[(*cnt)++] = (uint32_t)tmp;
}

static seq_t *A280866_sequence = NULL;
static size_t A280866_seq_len = 0, A280866_seq_cap = 0;
static unsigned char *A280866_used = NULL;
static size_t A280866_used_cap = 0;
static uint8_t *A280866_run_len = NULL;
static size_t A280866_run_len_cap = 0;
static bool A280866_initialized = false;

static void A280866_ensure_used_cap(idx_t n)
{
    if ((size_t)n < A280866_used_cap) return;
    size_t new_cap = A280866_used_cap ? A280866_used_cap * 2 : 128;
    if (new_cap <= (size_t)n) new_cap = (size_t)n + 64;
    unsigned char *tmp = (unsigned char*)calloc(new_cap, sizeof(unsigned char));
    if (A280866_used_cap) memcpy(tmp, A280866_used, A280866_used_cap);
    free(A280866_used);
    A280866_used = tmp;
    A280866_used_cap = new_cap;
}

static void A280866_ensure_prime_cap(uint32_t p)
{
    if (p < A280866_run_len_cap) return;
    size_t new_cap = A280866_run_len_cap ? A280866_run_len_cap * 2 : 128;
    while (new_cap <= p) new_cap <<= 1;
    uint8_t *tmp = (uint8_t*)calloc(new_cap, sizeof(uint8_t));
    if (A280866_run_len_cap) memcpy(tmp, A280866_run_len, A280866_run_len_cap);
    free(A280866_run_len);
    A280866_run_len = tmp;
    A280866_run_len_cap = new_cap;
}

static void A280866_init_sequence(void)
{
    if (A280866_initialized) return;
    A280866_used_cap = 128;
    A280866_used = (unsigned char*)calloc(A280866_used_cap, sizeof(unsigned char));
    A280866_run_len_cap = 128;
    A280866_run_len = (uint8_t*)calloc(A280866_run_len_cap, sizeof(uint8_t));
    A280866_seq_cap = 16;
    A280866_sequence = mem_new(seq_t, A280866_seq_cap);
    A280866_sequence[0] = (seq_t)1;
    A280866_seq_len = 1;
    A280866_used[1] = 1;
    A280866_initialized = true;
}

static seq_t A280866_a(idx_t n)
{
    A280866_init_sequence();
    while (A280866_seq_len < (size_t)n) {
        seq_t last = A280866_sequence[A280866_seq_len - 1];
        uint32_t last_pf[64];
        size_t last_cnt;
        A280866_get_prime_factors(last, last_pf, &last_cnt);
        for (size_t i = 0; i < last_cnt; ++i)
            A280866_ensure_prime_cap(last_pf[i]);

        idx_t candIdx = 2;
        while (1) {
            A280866_ensure_used_cap(candIdx);
            if (A280866_used[(size_t)candIdx]) { ++candIdx; continue; }
            bool ok = true;
            for (size_t i = 0; i < last_cnt && ok; ++i) {
                uint32_t p = last_pf[i];
                if (candIdx % p != 0) {
                    if (A280866_run_len[p] == 1) ok = false;
                }
            }
            if (!ok) { ++candIdx; continue; }

            seq_t cand = (seq_t)candIdx;
            uint32_t cand_pf[64];
            size_t cand_cnt;
            A280866_get_prime_factors(cand, cand_pf, &cand_cnt);
            for (size_t i = 0; i < cand_cnt; ++i)
                A280866_ensure_prime_cap(cand_pf[i]);

            for (size_t i = 0; i < last_cnt; ++i) {
                uint32_t p = last_pf[i];
                if (candIdx % p == 0) A280866_run_len[p] += 1;
                else A280866_run_len[p] = 0;
            }
            for (size_t i = 0; i < cand_cnt; ++i) {
                uint32_t p = cand_pf[i];
                if (candIdx % p == 0 && last % p != 0)
                    A280866_run_len[p] = 1;
            }
            if (A280866_seq_len >= A280866_seq_cap) {
                A280866_seq_cap = A280866_seq_cap * 2;
                A280866_sequence = mem_resize(seq_t, A280866_sequence, A280866_seq_cap);
            }
            A280866_sequence[A280866_seq_len++] = cand;
            A280866_used[(size_t)candIdx] = 1;
            break;
        }
    }
    return A280866_sequence[(size_t)n - 1];
}

static const uint32_t A372697_primes[64] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311
};

static seq_t A372697_A019565(idx_t n)
{
    seq_t result = 1;
    int bit = 0;
    while (n != 0) {
        if (n & 1) result *= (seq_t)A372697_primes[bit];
        n >>= 1;
        ++bit;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t target = A372697_A019565(n);
    for (idx_t k = 1; ; ++k) {
        if (A280866_a(k) == target) return (seq_t)k;
    }
}
