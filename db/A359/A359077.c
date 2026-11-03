/*
 * A359077  Numbers that do not have two proper divisors with an equal sum of
 *          digits.
 * https://oeis.org/A359077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-03)
 * Helpers:   digit_sum, factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Returns true if n has two proper divisors with an equal digit sum */
static bool has_duplicate_digit_sum_divisors(uint64_t n)
{
    if (n < 2) return false;  /* 1 has no proper divisors */

    unsigned char seen[400] = {0};  /* supports sums up to 9*39 < 400 */
    bool dup = false;

    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    /* enumerate all divisors except n itself */
    /* recursive stack-limited enumeration */
    typedef struct { int idx; uint64_t cur; } Frame;
    Frame stack[64];
    size_t sp = 0;
    stack[sp++] = (Frame){0, 1ULL};

    while (sp > 0) {
        Frame f = stack[--sp];
        if (f.idx == cnt) {
            if (f.cur == n) continue;  /* exclude n, but include 1 */
            unsigned int sd = (unsigned int)digit_sum((seq_t)f.cur);
            if (seen[sd]) { dup = true; break; }
            seen[sd] = 1;
            continue;
        }
        /* generate powers of primes[f.idx] */
        uint64_t p = primes[f.idx];
        uint64_t power = 1;
        for (uint64_t k = 0; k <= exps[f.idx]; ++k) {
            stack[sp++] = (Frame){f.idx + 1, f.cur * power};
            power *= p;
        }
    }

    return dup;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safe fallback */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of computed terms */
    static size_t cache_cap = 0;          /* allocated capacity */
    static uint64_t tested = 1;           /* next integer to test */

    /* initialise cache */
    if (cache == NULL) {
        grow_seq(&cache, &cache_cap, 16);  /* start with room for 16 terms */
    }

    /* Generate terms until we have the nth one */
    while (cache_len < (size_t)n) {
        if (!has_duplicate_digit_sum_divisors(tested)) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)tested;
        }
        ++tested;
    }
    return cache[n - 1];
}
