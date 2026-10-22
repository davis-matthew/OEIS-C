/*
 * A371454  Numbers whose binary indices are all semiprimes.
 * https://oeis.org/A371454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th integer whose binary representation uses only
   semiprime indices.  For n ≥ 1:
   a(n) = Σ_{j≥0} bit_j(n) · 2^{s_j – 1},
   where s_j is the j‑th semiprime (4,6,9,10,14,15,…). */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Pre‑compute the first MAX_SEMIPRIMES semiprime numbers (ascending). */
/* 256 is more than enough for all 64‑bit indices (we need < 64 of them). */
#define MAX_SEMIPRIMES 256
static uint16_t semiprime_positions[MAX_SEMIPRIMES];
static size_t semiprime_count = 0;
static bool semiprimes_initialized = false;

static int cmp_uint32(const void *a, const void *b)
{
    uint32_t ua = *(const uint32_t*)a;
    uint32_t ub = *(const uint32_t*)b;
    return (ua < ub) ? -1 : (ua > ub) ? 1 : 0;
}

/* Build a table of semiprime integers s_i (s_0 = 4, …). */
static void init_semiprimes(void)
{
    if (semiprimes_initialized)
        return;
    const uint16_t LIMIT = 5000;            /* enough to collect many semiprimes */
    bool is_prime[LIMIT + 1];
    for (uint16_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint16_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint16_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    /* collect all semiprimes ≤ LIMIT */
    uint32_t tmp[20000];
    size_t tmpcnt = 0;
    for (uint16_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint16_t j = i; j <= LIMIT; ++j) {
                if (is_prime[j]) {
                    uint32_t prod = (uint32_t)i * j;
                    if (prod > LIMIT)
                        break;
                    tmp[tmpcnt++] = prod;
                }
            }
        }
    }
    /* sort and keep only unique values */
    qsort(tmp, tmpcnt, sizeof(uint32_t), cmp_uint32);
    size_t k = 0;
    for (size_t i = 0; i < tmpcnt; ++i) {
        if (i == 0 || tmp[i] != tmp[i - 1]) {
            if (k < MAX_SEMIPRIMES)
                semiprime_positions[k++] = (uint16_t)tmp[i];
        }
    }
    semiprime_count = k;
    semiprimes_initialized = true;
}

/* Return the n-th term of OEIS sequence A371454 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    init_semiprimes();
    seq_t val = 0;
    idx_t m = n;
    size_t bit = 0;
    while (m && bit < semiprime_count) {
        if (m & 1ULL) {
            val += pow2((idx_t)(semiprime_positions[bit] - 1));
        }
        m >>= 1;
        ++bit;
    }
    return val;
}
