/*
 * A393252  Number of partitions of A083207(n) into distinct Zumkeller
 *          numbers.
 * https://oeis.org/A393252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of A083207(n) into distinct
 *            Zumkeller numbers (A083207); computed by generating Zumkeller
 *            numbers and a 0/1 knapsack counting DP
 * Range:     u8 n <= 37, u16 n <= 95, u32 n <= 277, u64 n <= 883,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 945 (2026-11-03)
 * Helpers:   factor_uint64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_uint64.h"
#include "../utils/mem.h"

static void A083207_gen_divs_helper(int idx, uint64_t cur, const uint64_t *fac,
                                    const int *exp, int cnt, uint64_t *arr, size_t *pos)
{
    if (idx == cnt) {
        arr[(*pos)++] = cur;
        return;
    }
    uint64_t val = cur;
    for (int e = 0; e <= exp[idx]; ++e) {
        A083207_gen_divs_helper(idx + 1, val, fac, exp, cnt, arr, pos);
        val *= fac[idx];
    }
}

static bool A083207_is_zumkeller(uint64_t n)
{
    if (n <= 2) return false;

    uint64_t fac[20];
    int exp[20];
    int cnt = 0;
    factor_uint64(n, fac, exp, &cnt);
    if (cnt == 0) return false;

    unsigned __int128 sigma = 1;
    int tau = 1;
    for (int i = 0; i < cnt; ++i) {
        uint64_t p = fac[i];
        int e = exp[i];
        unsigned __int128 p_pow = 1, sum_p = 1;
        for (int j = 0; j < e; ++j) {
            p_pow *= p;
            sum_p += p_pow;
        }
        sigma *= sum_p;
        tau *= (e + 1);
    }
    if ((sigma & 1) != 0) return false;
    uint64_t target = (uint64_t)(sigma >> 1);

    uint64_t *divs = (uint64_t *)calloc((size_t)tau, sizeof(uint64_t));
    if (!divs) abort();
    size_t div_cnt = 0;
    A083207_gen_divs_helper(0, 1ULL, fac, exp, cnt, divs, &div_cnt);

    size_t bits_len = (size_t)(target >> 6) + 1;
    uint64_t *bits = (uint64_t *)calloc(bits_len, sizeof(uint64_t));
    if (!bits) { free(divs); return false; }
    bits[0] = 1ULL;

    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        if (d > target) continue;
        unsigned word_shift = (unsigned)(d >> 6);
        unsigned bit_shift = (unsigned)(d & 63ULL);
        if (word_shift >= bits_len) continue;
        for (size_t j = bits_len; j-- > 0; ) {
            uint64_t val = bits[j];
            if (!val) continue;
            size_t k = j + word_shift;
            if (k >= bits_len) continue;
            if (bit_shift == 0) {
                bits[k] |= val;
            } else {
                bits[k] |= val << bit_shift;
                if (k + 1 < bits_len)
                    bits[k + 1] |= val >> (64 - bit_shift);
            }
        }
    }
    bool ok = ((bits[target >> 6] >> (target & 63ULL)) & 1ULL) != 0;
    free(bits);
    free(divs);
    return ok;
}

static uint64_t *Z_list = NULL;
static size_t Z_len = 0, Z_cap = 0;
static uint64_t Z_last = 5;

static void Z_extend(size_t n)
{
    while (Z_len < n) {
        ++Z_last;
        if (A083207_is_zumkeller(Z_last)) {
            if (Z_len == Z_cap) {
                Z_cap = Z_cap ? Z_cap * 2 : 64;
                Z_list = (uint64_t *)realloc(Z_list, Z_cap * sizeof(uint64_t));
                if (!Z_list) abort();
            }
            Z_list[Z_len++] = Z_last;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    Z_extend((size_t)n);
    size_t N = (size_t)Z_list[n - 1];

    seq_t *dp = mem_new(seq_t, N + 1);
    dp[0] = 1;
    for (size_t i = 0; i < Z_len; ++i) {
        size_t z = (size_t)Z_list[i];
        if (z > N) break;
        for (size_t s = N; s >= z; --s)
            dp[s] += dp[s - z];
    }
    seq_t r = dp[N];
    mem_free(dp);
    return r;
}
