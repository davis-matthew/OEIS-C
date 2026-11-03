/*
 * A257500  Numbers that cannot be expressed as the sum of distinct odd
 *          composite numbers.
 * https://oeis.org/A257500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: full, pass, 31 terms through n = 31 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numbers that cannot be expressed as a sum of
   distinct odd composite numbers.
   For n ≥ 1, a(n) returns the n‑th term of the sequence. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static void build_cache(seq_t **p_cache, size_t *p_len)
{
    const idx_t LIMIT = 10000;            /* safe bound for all terms */
    const size_t WORDS = (LIMIT >> 6) + 1;/* number of 64‑bit words */

    /* 1. Sieve of Eratosthenes up to LIMIT */
    uint8_t *is_prime = (uint8_t*)calloc(LIMIT + 1, sizeof(uint8_t));
    if (!is_prime) exit(1);
    for (idx_t i = 2; i <= LIMIT; ++i) is_prime[i] = 1;
    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;

    /* 2. Collect odd composite numbers ≤ LIMIT */
    idx_t *composites = (idx_t*)malloc(LIMIT * sizeof(idx_t)); /* over‑allocate */
    if (!composites) exit(1);
    size_t comp_cnt = 0;
    for (idx_t x = 9; x <= LIMIT; x += 2)
        if (!is_prime[x]) composites[comp_cnt++] = x;

    /* 3. Subset‑sum DP for representable sums */
    uint64_t *rep = (uint64_t*)calloc(WORDS, sizeof(uint64_t));
    if (!rep) exit(1);
    rep[0] = 1ULL;                          /* sum 0 is achievable */

    for (size_t i = 0; i < comp_cnt; ++i) {
        idx_t v = composites[i];
        size_t shift_words = v >> 6;
        uint64_t shift_bits = v & 63ULL;

        if (shift_words >= WORDS) continue; /* shift beyond bitset size */

        for (int64_t w = WORDS - 1; w >= (int64_t)shift_words; --w) {
            uint64_t low  = rep[w - shift_words] << shift_bits;
            uint64_t high = 0;
            if (shift_bits && w - shift_words > 0)
                high = rep[w - shift_words - 1] >> (64 - shift_bits);
            rep[w] |= low | high;
        }
    }

    /* 4. Extract numbers 1..LIMIT that are NOT representable */
    size_t cap = 64;
    seq_t *cache = mem_new(seq_t, cap);
    if (!cache) exit(1);
    size_t len = 0;

    for (idx_t x = 1; x <= LIMIT; ++x) {
        size_t w = x >> 6;
        uint64_t bit = x & 63ULL;
        if (!(rep[w] >> bit & 1ULL)) {
            if (len >= cap) {
                cap *= 2;
                seq_t *tmp = mem_resize(seq_t, cache, cap);
                if (!tmp) exit(1);
                cache = tmp;
            }
            cache[len++] = (seq_t)x;
        }
    }

    /* Clean up temporary storage */
    free(is_prime);
    free(composites);
    free(rep);

    *p_cache = cache;
    *p_len   = len;
}

/* --------------------------------------------------------------------------
   1.  Compute the list of non‑representable numbers once.
   2.  All required numbers are ≤ 53, so a bound of 10000 is safely
       sufficient for the entire finite sequence.
   3.  The algorithm builds a DP bitset of representable sums using
       all odd composite numbers ≤ 10000.
   -------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------
   Public entry point required by the build system.
   -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *nonrep = NULL;
    static size_t nonrep_len = 0;

    if (!initialized) {
        build_cache(&nonrep, &nonrep_len);
        initialized = true;
    }

    if (n == 0) return 0;            /* index zero not defined */
    if (n - 1 < nonrep_len)
        return nonrep[n - 1];
    else
        return 0;                    /* out of range; sequence is finite */
}
