/*
 * A345601  Numbers that are the sum of ten fourth powers in eight or more
 *          ways.
 * https://oeis.org/A345601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A345601: Numbers that are the sum of ten fourth powers in eight or more ways */
/* The function builds all such numbers up to a fixed maximum sum once,
   then returns the n‑th term (1‑based).  The table is stored in a static
   array and the routine reuses it for subsequent calls. */

#include <stdint.h>
#include <stdlib.h>

static seq_t *terms = NULL;          /* array of found terms */
static size_t terms_len = 0;         /* number of terms stored */
static int terms_initialized = 0;    /* have we built the table? */

/* Build the table of terms once */
static void build_terms(void)
{
    /* Maximum sum to consider (enough for first ~10 000 terms) */
    const size_t MAX_SUM = 1500000ull;
    const size_t MAX_T   = 10ull;      /* exactly 10 summands */

    /* ---------- Compute all fourth powers ≤ MAX_SUM ---------- */
    uint64_t *powers = (uint64_t*)NULL;
    size_t power_cnt = 0;
    for (uint64_t i = 1; ; ++i) {
        uint64_t v = i*i*i*i;        /* i⁴ */
        if (v > MAX_SUM) break;
        powers = (uint64_t*)realloc(powers, (power_cnt + 1)*sizeof(uint64_t));
        powers[power_cnt++] = v;
    }

    /* ---------- DP table: dp[t][s] = #ways to write s as sum of t fourth powers ---------- */
    const size_t stride = MAX_SUM + 1;
    const size_t dp_size = (MAX_T + 1) * stride;
    uint8_t *dp = (uint8_t*)malloc(dp_size);
    if (!dp) abort();
    for (size_t i = 0; i < dp_size; ++i) dp[i] = 0;
    dp[0] = 1;                       /* 0 uses zero terms */

    for (size_t p = 0; p < power_cnt; ++p) {
        size_t v = powers[p];
        for (size_t t = 1; t <= MAX_T; ++t) {
            size_t offset_t   = t * stride;
            size_t offset_t_1 = (t - 1) * stride;
            for (size_t s = v; s <= MAX_SUM; ++s) {
                uint8_t add = dp[offset_t_1 + s - v];
                if (!add) continue;
                uint8_t cur  = dp[offset_t + s];
                uint8_t nc   = cur + add;
                dp[offset_t + s] = (nc > 8 ? 8 : nc);  /* saturate at 8 */
            }
        }
    }

    /* ---------- Extract sums with ≥8 representations ---------- */
    size_t cap = 200000;                      /* initial capacity */
    terms = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!terms) abort();

    for (size_t s = 0; s <= MAX_SUM; ++s) {
        if (dp[MAX_T * stride + s] >= 8) {
            if (terms_len >= cap) {
                cap *= 2;
                terms = (seq_t*)realloc(terms, cap * sizeof(seq_t));
                if (!terms) abort();
            }
            terms[terms_len++] = (seq_t)s;
        }
    }

    /* ---------- Cleanup ---------- */
    free(powers);
    free(dp);
    terms_initialized = 1;
}

/* Public function required by the task */
seq_t a(idx_t n)
{
    if (!terms_initialized) build_terms();

    /* n is 1‑based; return 0 if out of range */
    if (n == 0 || (size_t)(n - 1) >= terms_len) return (seq_t)0;

    return terms[(size_t)(n - 1)];
}
