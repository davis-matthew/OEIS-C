/*
 * A352104  a(n) is the number of 1's in the maximal tribonacci representation
 *          of n (A352103).
 * https://oeis.org/A352104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of ones in the maximal tribonacci representation
 *            of n Computed via a 0/1 subset‑sum dynamic programming that
 *            maximises the size of the subset of distinct tribonacci numbers
 *            whose sum equals n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   free_dp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/free_dp.h"

/* Forward declaration of the required function */
seq_t a(idx_t n);

/* internal helpers ------------------------------------------------------ */
static void shift_or(uint64_t *dest, const uint64_t *src, idx_t shift, size_t len)
{
    size_t wshift = (size_t)(shift / 64);
    unsigned int bshift = (unsigned int)(shift % 64);
    if (wshift >= len) return;
    if (bshift == 0) {
        for (size_t i = len; i-- > wshift; ) {
            dest[i] |= src[i - wshift];
        }
    } else {
        for (size_t i = len; i-- > wshift+1; ) {
            dest[i] |= (src[i - wshift] << bshift)
                    | (src[i - wshift - 1] >> (64 - bshift));
        }
        dest[wshift] |= (src[0] << bshift);
    }
}

/* DP data --------------------------------------------------------------- */
static idx_t *trib_vals          = NULL;  /* distinct tribonacci values   */
static size_t trib_cnt           = 0;     /* number of tribonacci values */
static uint64_t **dp_bits        = NULL;   /* dp_bits[k][i] holds bitset    */
static size_t dp_len             = 0;     /* length of bitset array       */
static idx_t  dp_max_n           = 0;     /* maximum n for which dp built */

static void recompute_dp(idx_t new_max)
{
    /* free old structures if any */
    if (dp_bits != NULL) {
        free_dp(dp_bits, trib_cnt + 1, dp_len);
        free(trib_vals);
        dp_bits = NULL;
        trib_vals = NULL;
    }

    dp_max_n = new_max;

    /* generate distinct tribonacci numbers <= new_max */
    idx_t a = 0, b = 1, c = 1;
    idx_t next = c;  /* start with T3 = 2 */
    next = a + b + c; /* first actual tribonacci >0 after 1,1 */
    /* build list */
    idx_t *tmp = NULL;
    size_t tmp_cap = 16;
    tmp = (idx_t *)malloc(tmp_cap * sizeof(idx_t));
    if (!tmp) exit(1);
    /* insert first distinct tribonacci numbers: 1,2,4,7,... */
    if (new_max >= 1) {
        tmp[0] = 1; trib_cnt = 1;
    }
    else {
        trib_cnt = 0;
    }
    /* generate further terms */
    while (1) {
        a = b; b = c; c = next;
        next = a + b + c;
        if (c > new_max) break;
        /* skip duplicate 1 already added */
        if (trib_cnt > 0 && c == tmp[trib_cnt - 1]) continue;
        if (trib_cnt >= tmp_cap) {
            tmp_cap <<= 1;
            tmp = (idx_t *)realloc(tmp, tmp_cap * sizeof(idx_t));
            if (!tmp) exit(1);
        }
        tmp[trib_cnt++] = c;
    }
    /* allocate trib_vals */
    trib_vals = (idx_t *)malloc(trib_cnt * sizeof(idx_t));
    if (!trib_vals) exit(1);
    for (size_t i = 0; i < trib_cnt; ++i) trib_vals[i] = tmp[i];
    free(tmp);

    dp_len = (size_t)(new_max / 64) + 1;
    dp_bits = (uint64_t **)malloc((trib_cnt + 1) * sizeof(uint64_t *));
    if (!dp_bits) exit(1);
    for (size_t k = 0; k <= trib_cnt; ++k) {
        dp_bits[k] = (uint64_t *)calloc(dp_len, sizeof(uint64_t));
        if (!dp_bits[k]) exit(1);
    }
    /* base case: sum 0 achievable with 0 coins */
    dp_bits[0][0] = 1ULL;

    /* DP: for each coin value */
    for (size_t idx = 0; idx < trib_cnt; ++idx) {
        idx_t val = trib_vals[idx];
        for (size_t k = trib_cnt; k >= 1; --k) {
            shift_or(dp_bits[k], dp_bits[k - 1], val, dp_len);
        }
    }
}

/* Public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n > dp_max_n) recompute_dp(n);
    size_t word = (size_t)(n / 64);
    unsigned int bit  = (unsigned int)(n % 64);
    uint64_t mask = 1ULL << bit;
    for (size_t k = trib_cnt; k > 0; --k) {
        if (dp_bits[k][word] & mask)
            return (seq_t)k;
    }
    return (seq_t)0;
}
