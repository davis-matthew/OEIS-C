/*
 * A391004  Primes p that can be represented as the sum of distinct nontrivial
 *          divisors of p + 1.
 * https://oeis.org/A391004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 2276, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, generate_all_divisors, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for primes p such that p can be written as the sum
   of distinct non‑trivial divisors of p + 1. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"     /* bool is_prime(idx_t x); */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/generate_all_divisors.h"

static bool prime_has_property(uint64_t p);
static void generate_all_divisors(const uint64_t *pr, const uint64_t *ex,
                                  size_t idx, size_t cnt, uint64_t cur,
                                  uint64_t **vec, size_t *len, size_t *cap);

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;      /* cached terms */
    static size_t seq_len = 0;     /* number of cached terms */
    static size_t seq_cap = 0;     /* allocated capacity */
    static uint64_t last_checked = 2;  /* next integer to test */

    if (n == 0) return 0;  /* undefined for n = 0 (offset 1) */

    while (seq_len < (size_t)n) {
        /* find next prime */
        while (!is_prime((idx_t)last_checked)) ++last_checked;

        uint64_t p = last_checked;
        if (prime_has_property(p)) {
            /* store the prime */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)p;
        }
        ++last_checked;  /* advance to look for the next prime */
    }

    return seq[n - 1];
}

static bool prime_has_property(uint64_t p)
{
    uint64_t d = p + 1;                /* number whose divisors are used */
    size_t cnt;
    uint64_t pr[64], ex[64];
    factor_u64(d, pr, ex, &cnt);

    /* generate all divisors */
    uint64_t *divs = NULL;
    size_t cap = 0, len = 0;
    generate_all_divisors(pr, ex, 0, cnt, 1, &divs, &len, &cap);

    /* filter nontrivial divisors: >1 and < d */
    size_t i, new_len = 0;
    for (i = 0; i < len; ++i) {
        uint64_t v = divs[i];
        if (v > 1 && v != d) {
            if (new_len != i) divs[new_len] = v;
            ++new_len;
        }
    }
    len = new_len;

    if (len == 0) {                /* no valid divisors */
        free(divs);
        return false;
    }

    /* Subset‑sum DP with a bitset */
    uint64_t target = p;
    size_t words = (target >> 6) + 1;
    uint64_t *bits = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!bits) abort();
    bits[0] = 1ULL;   /* sum 0 reachable */

    for (i = 0; i < len; ++i) {
        uint64_t val = divs[i];
        if (val > target) continue;
        size_t shift = val;     /* shift amount */
        size_t wshift = shift >> 6;
        int    bshift = shift & 63;

        for (size_t idx = words; idx-- > 0; ) {
            uint64_t add = 0;
            if (idx >= wshift) {
                add = bits[idx - wshift] << bshift;
                if (bshift && idx - wshift > 0)
                    add |= bits[idx - wshift - 1] >> (64 - bshift);
            }
            bits[idx] |= add;
        }
    }

    bool ok = ((bits[target >> 6] >> (target & 63)) & 1ULL) != 0;

    free(bits);
    free(divs);
    return ok;
}
