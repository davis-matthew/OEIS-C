/*
 * A376880  Numbers that have Zumkeller divisors.
 * https://oeis.org/A376880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes A376880 – Numbers that have Zumkeller divisors.
 * The function generates the sequence lazily: it keeps a static
 * list of found terms and expands it until the requested index is
 * reached.  For each integer it tests whether the number itself
 * or any of its proper divisors is Zumkeller.  A number is
 * Zumkeller iff the sum of its divisors is even and a subset of
 * the divisors sums to half that total.  This subset‑sum test is
 * performed with a bitset and is memoised.
 *
 * All helper functions that are part of the provided build
 * library are used via the corresponding header files.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* Return all positive divisors of n (including 1 and n).  The caller
   must free(*divs).  The divisors are returned in arbitrary order. */
static void get_divisors(uint64_t n, uint64_t **divs, size_t *cnt)
{
    uint64_t tmp = n;
    uint64_t *primes = (uint64_t*)malloc(16 * sizeof(uint64_t));
    int *exps   = (int*)malloc(16 * sizeof(int));
    size_t pf_cnt = 0, pf_cap = 16;

    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            int e = 0;
            while (tmp % d == 0) { tmp /= d; ++e; }
            if (pf_cnt == pf_cap) {
                pf_cap <<= 1;
                primes = (uint64_t*)realloc(primes, pf_cap * sizeof(uint64_t));
                exps   = (int*)realloc(exps, pf_cap * sizeof(int));
            }
            primes[pf_cnt] = d;
            exps[pf_cnt]   = e;
            ++pf_cnt;
        }
    }
    if (tmp > 1) {
        if (pf_cnt == pf_cap) {
            pf_cap <<= 1;
            primes = (uint64_t*)realloc(primes, pf_cap * sizeof(uint64_t));
            exps   = (int*)realloc(exps, pf_cap * sizeof(int));
        }
        primes[pf_cnt] = tmp;
        exps[pf_cnt]   = 1;
        ++pf_cnt;
    }

    /* generate all divisors */
    size_t div_cap = 16;
    uint64_t *dv = (uint64_t*)malloc(div_cap * sizeof(uint64_t));
    dv[0] = 1;
    size_t div_cnt = 1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        int e      = exps[i];
        size_t old_cnt = div_cnt;
        for (size_t j = 0; j < old_cnt; ++j) {
            uint64_t val = dv[j];
            for (int k = 1; k <= e; ++k) {
                val *= p;
                if (div_cnt == div_cap) {
                    div_cap <<= 1;
                    dv = (uint64_t*)realloc(dv, div_cap * sizeof(uint64_t));
                }
                dv[div_cnt++] = val;
            }
        }
    }

    free(primes);
    free(exps);

    *divs = dv;
    *cnt  = div_cnt;
}

/* Memoised cache for Zumkeller status: -1 = unknown, 0 = false, 1 = true. */
static signed char *z_cache = NULL;
static size_t z_cap = 0;

static void ensure_z_cache_capacity(size_t n)
{
    if (n < z_cap) return;
    size_t new_cap = z_cap ? z_cap << 1 : 16;
    while (new_cap <= n) new_cap <<= 1;
    signed char *new_arr = (signed char*)realloc(z_cache, new_cap * sizeof(signed char));
    if (!new_arr) exit(1);
    for (size_t i = z_cap; i < new_cap; ++i) new_arr[i] = -1;
    z_cache = new_arr;
    z_cap = new_cap;
}

static bool compute_is_zumkeller(uint64_t n);

/* Public helper that uses memoisation */
static bool is_zumkeller_helper(uint64_t n)
{
    ensure_z_cache_capacity(n);
    if (z_cache[n] != -1) return z_cache[n];
    bool res = compute_is_zumkeller(n);
    z_cache[n] = res ? 1 : 0;
    return res;
}

/* Compute whether the integer n is a Zumkeller number. */
static bool compute_is_zumkeller(uint64_t n)
{
    uint64_t *divs = NULL;
    size_t div_cnt = 0;
    get_divisors(n, &divs, &div_cnt);

    uint64_t sigma = 0;
    for (size_t i = 0; i < div_cnt; ++i) sigma += divs[i];
    bool result = false;

    if ((sigma & 1ULL) == 0) {
        uint64_t target = sigma >> 1;
        size_t words = (target >> 6) + 1;
        uint64_t *bits = (uint64_t*)calloc(words, sizeof(uint64_t));
        bits[0] = 1ULL;   /* sum 0 is always achievable */

        for (size_t i = 0; i < div_cnt && !result; ++i) {
            uint64_t d = divs[i];
            if (d > target) continue;

            size_t wshift = d >> 6;
            unsigned int bshift = d & 63;
            uint64_t *shifted = (uint64_t*)calloc(words, sizeof(uint64_t));
            for (size_t w = 0; w < words; ++w) {
                uint64_t val = bits[w];
                if (!val) continue;
                size_t dst = w + wshift;
                if (dst >= words) continue;
                shifted[dst] |= val << bshift;
                if (bshift && dst + 1 < words) {
                    shifted[dst + 1] |= val >> (64 - bshift);
                }
            }
            for (size_t w = 0; w < words; ++w) bits[w] |= shifted[w];
            free(shifted);
            size_t word_index = target >> 6;
            unsigned int bit_index = target & 63;
            if (bits[word_index] & (1ULL << bit_index)) result = true;
        }
        free(bits);
    }

    free(divs);
    return result;
}

/* Checks whether n has at least one Zumkeller divisor distinct from 1. */
static bool has_zumkeller_divisor(uint64_t n)
{
    if (is_zumkeller_helper(n)) return true;

    uint64_t *divs = NULL;
    size_t cnt = 0;
    get_divisors(n, &divs, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t d = divs[i];
        if (d == 1 || d == n) continue;
        if (is_zumkeller_helper(d)) {
            free(divs);
            return true;
        }
    }
    free(divs);
    return false;
}

/* Storage for the growing sequence */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t last_checked = 0;

/* Main function: return the n‑th (1‑based) term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety guard – not used by OEIS */

    while (seq_len < (size_t)n) {
        ++last_checked;
        if (has_zumkeller_divisor(last_checked)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap << 1 : 16;
                seq_t *new_seq = seq_realloc(seq, seq_cap, new_cap);
                if (!new_seq) exit(1);
                seq = new_seq;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)last_checked;
        }
    }
    return seq[n - 1];
}
