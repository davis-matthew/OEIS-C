/*
 * A390329  Numbers k such that the number of distinct prime divisors of
 *          binomial(2k, k) is even.
 * https://oeis.org/A390329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   build_primes_upto, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A390329: a(n) is the nth integer k such that the number of distinct
   prime divisors of binomial(2k,k) is even. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/build_primes_upto.h"

/* v_p(n!) = sum_{i>=1} floor(n / p^i) */
static idx_t vp_factorial(idx_t n, uint64_t p)
{
    idx_t sum = 0;
    idx_t pp = p;
    while (pp <= n) {
        sum += n / pp;
        if (pp > n / p) break;
        pp *= p;
    }
    return sum;
}

/* global state for memoisation */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit_needed = 0;

static seq_t *seq_vals   = NULL;
static size_t seq_vals_len = 0;
static size_t seq_vals_cap = 0;
static idx_t last_k_processed = 0;

/* make sure we have primes up to limit */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit_needed) return;
    free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit_needed = limit;
}

/* generate terms up to and including limit_k */
static void generate_up_to(idx_t limit_k)
{
    if (limit_k <= last_k_processed) return;

    /* we need primes up to 2*limit_k for factorisation */
    idx_t prime_need = limit_k << 1;
    ensure_primes(prime_need);

    /* allocate seq_vals if it is the first time */
    if (seq_vals == NULL) {
        seq_vals_cap = 16;
        seq_vals = seq_alloc(seq_vals_cap);
    }

    for (idx_t k = last_k_processed + 1; k <= limit_k; ++k) {
        int parity = 0;                 /* 0 = even, 1 = odd */
        idx_t twok = k << 1;

        for (size_t pi = 0; pi < prime_count && prime_list[pi] <= twok; ++pi) {
            uint64_t p = prime_list[pi];
            idx_t a = vp_factorial(twok, p);
            idx_t b = vp_factorial(k, p);
            idx_t exp = a - 2 * b;
            if (exp > 0) parity ^= 1;
        }

        if (parity == 0) {            /* even number of distinct primes */
            if (seq_vals_len == seq_vals_cap) {
                size_t new_cap = seq_vals_cap * 2;
                seq_vals = seq_realloc(seq_vals, seq_vals_cap, new_cap);
                seq_vals_cap = new_cap;
            }
            seq_vals[seq_vals_len++] = (seq_t)k;
        }
    }

    last_k_processed = limit_k;
}

/* public function required by the judge */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, guard against underflow */

    /* generate more terms until we have at least n entries */
    while (seq_vals_len < n) {
        idx_t new_limit = (last_k_processed == 0) ? 4 : last_k_processed * 2;
        generate_up_to(new_limit);
    }

    return seq_vals[n - 1];
}
