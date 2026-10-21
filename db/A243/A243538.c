/*
 * A243538  Numbers n such that the list of divisors of n contains 5 distinct
 *          digits (in base 10).
 * https://oeis.org/A243538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 5192, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer whose divisor set contains exactly 5 distinct decimal digits. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_realloc.h"

struct seq_state {
    idx_t          next_candidate;   /* next number to test */
    seq_t *        cache;            /* cached terms */
    size_t         sz;               /* number of cached terms */
    size_t         cap;              /* capacity of cache */
    uint64_t *     primes;           /* list of primes for factorisation */
    size_t         primes_cnt;
} static state = { 1ULL, NULL, 0, 0, NULL, 0 };

/* Initialise primes once.  A bound of 200 000 guarantees enough primes
   for the first 10 000 terms. */
static void init_primes(void)
{
    if (state.primes != NULL) return;
    const idx_t limit = 200000;
    size_t cnt = 0;
    build_primes_upto(limit, &state.primes, &cnt);
    state.primes_cnt = cnt;
}

/* Recursively generate all divisors from the factorisation primes `pr`
   and exponents `ex`.  Count the distinct decimal digits of all divisors. */
static void generate_divisors(size_t idx,
                              size_t cnt,
                              uint64_t cur,
                              const uint64_t *pr,
                              const uint64_t *ex,
                              bool *seen,
                              int *distinct)
{
    if (idx == cnt) {
        uint64_t d = cur;
        while (d > 0) {
            int dig = d % 10;
            if (!seen[dig]) { seen[dig] = true; (*distinct)++; }
            d /= 10;
        }
        return;
    }
    uint64_t val = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        generate_divisors(idx + 1, cnt, val, pr, ex, seen, distinct);
        val *= pr[idx];
    }
}

/* Return true if the divisors of `n` contain exactly five distinct digits. */
static bool has_five_digit_divisors(uint64_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);

    bool seen[10] = { false };
    int distinct = 0;
    generate_divisors(0, cnt, 1, pr, ex, seen, &distinct);
    return distinct == 5;
}

/* Main sequence function.  Memoises all earlier terms. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1 */

    init_primes();

    while (state.sz < (size_t)n) {
        if (has_five_digit_divisors(state.next_candidate)) {
            if (state.cap <= state.sz) {
                size_t new_cap = state.cap ? state.cap * 2 : 16;
                if (new_cap < state.sz + 1) new_cap = state.sz + 1;
                state.cache = seq_realloc(state.cache, state.cap, new_cap);
                state.cap = new_cap;
            }
            state.cache[state.sz] = (seq_t)state.next_candidate;
            state.sz++;
        }
        state.next_candidate++;
    }
    return state.cache[n - 1];
}
