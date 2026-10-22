/*
 * A383412  Lexicographically earliest sequence of integers >= 2 such that
 *          whenever a(k_1) = ... = a(k_m) with k_1 < ... < k_m, the sum k_1 +
 *          ... + k_m can be computed without carries in base a(k_1).
 * https://oeis.org/A383412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=2; for n>0 the least integer ≥2 such that for every value v
 *            that appears at indices I_v, the sum of the digits (in base v)
 *            of all indices in I_v is ≤ v‑1. This guarantees that any subset
 *            of equal values has a carry‑free sum in base v. The algorithm
 *            builds the sequence lexicographically by te...
 * Range:     u8 n <= 2850, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration matching the required interface */
seq_t a(idx_t n);

/* Internal structures ----------------------------------------------------- */

typedef struct {
    uint64_t *sums;   /* per‑digit cumulative sums */
    size_t   len;     /* number of digit positions stored */
} ValueInfo;

/* Global static state ----------------------------------------------------- */

static seq_t   *a_seq    = NULL; /* computed sequence values          */
static size_t  a_len     = 0;
static size_t  a_cap     = 0;

static ValueInfo **valueInfos = NULL; /* array indexed by the value v   */
static size_t      value_cap   = 0;

/* Helper functions -------------------------------------------------------- */

static void ensure_a_capacity(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    a_seq = (seq_t *)realloc(a_seq, new_cap * sizeof(seq_t));
    a_cap = new_cap;
}

static void ensure_value_infos_capacity(uint32_t v)
{
    if (v < value_cap) return;
    size_t new_cap = value_cap ? value_cap * 2 : 8;
    while (new_cap <= v) new_cap *= 2;
    ValueInfo **new_ptr = (ValueInfo **)realloc(valueInfos,
                                                new_cap * sizeof(ValueInfo *));
    /* initialize newly added slots to NULL */
    for (size_t i = value_cap; i < new_cap; ++i) new_ptr[i] = NULL;
    valueInfos = new_ptr;
    value_cap  = new_cap;
}

static bool can_use(idx_t n, uint32_t v)
{
    ValueInfo *info = (v < value_cap) ? valueInfos[v] : NULL;
    uint64_t  temp  = n;
    size_t    len   = info ? info->len : 0;

    for (size_t pos = 0; temp > 0 || pos < len; ++pos) {
        uint64_t digit = (temp > 0) ? (temp % v) : 0;
        if (temp > 0) temp /= v;
        uint64_t current = (info && pos < info->len) ? info->sums[pos] : 0;
        if (current + digit > (uint64_t)(v - 1)) return false;
    }
    return true;
}

static void record_use(idx_t n, uint32_t v)
{
    ensure_value_infos_capacity(v);
    ValueInfo *info = valueInfos[v];
    if (!info) {
        info = (ValueInfo *)malloc(sizeof(ValueInfo));
        info->len  = 0;
        info->sums = NULL;
        valueInfos[v] = info;
    }

    uint64_t temp = n;
    size_t   pos  = 0;
    while (temp > 0 || pos < info->len) {
        uint64_t digit = (temp > 0) ? (temp % v) : 0;
        if (temp > 0) temp /= v;

        if (pos >= info->len) {
            size_t new_len = pos + 1;
            uint64_t *new_sums = (uint64_t *)realloc(info->sums,
                                                     new_len * sizeof(uint64_t));
            /* initialise new slots to 0 */
            for (size_t t = info->len; t < new_len; ++t) new_sums[t] = 0;
            info->sums = new_sums;
            info->len  = new_len;
        }
        info->sums[pos] += digit;
        ++pos;
    }
}

/* Main function ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Ensure the sequence has been computed up to index n. */
    while (a_len <= n) {
        if (a_len == 0) {          /* initialise a(0) = 2 */
            ensure_a_capacity(1);
            a_seq[0] = 2;
            record_use(0, 2);
            ++a_len;
            continue;
        }
        idx_t idx = a_len;
        uint32_t v_candidate = 2;

        /* Find the smallest admissible value for this index. */
        while (true) {
            if (can_use(idx, v_candidate)) {
                ensure_a_capacity(idx + 1);
                a_seq[idx] = v_candidate;
                record_use(idx, v_candidate);
                break;
            }
            ++v_candidate;
        }
        ++a_len;
    }

    return a_seq[n];
}
