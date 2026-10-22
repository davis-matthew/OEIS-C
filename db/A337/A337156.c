/*
 * A337156  Numbers k such that the k-th triangular number has all its prime
 *          factors congruent to 1 mod 4.
 * https://oeis.org/A337156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 921, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find k ≡ 1 (mod 8) such that both k and (k+1)/2 have all
   prime factors ≡ 1 (mod 4).  The returned value is the kth such number. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* static cache of computed terms */
static seq_t *seq_arr  = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

/* static cache of primes for factor checks */
static uint64_t *primes   = NULL;
static size_t    prime_cnt = 0;
static bool      primes_inited = false;

/* initialise prime table up to some limit; this limit covers
   all k that occur for n up to at least 10 000. */
static void init_primes(void)
{
    if (primes_inited) return;
    const idx_t limit = 2000000;          /* 2 × 10⁶  → primes up to 2 M */
    build_primes_upto(limit, (uint64_t**)&primes, &prime_cnt);
    primes_inited = true;
}

/* test whether all prime divisors of n are ≡ 1 (mod 4) */
static bool has_only_1mod4_factors(uint64_t n)
{
    if (n <= 1) return true;             /* 1 is allowed */

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0)
        {
            if (p % 4 != 1) return false;
            while (n % p == 0) n /= p;
        }
    }
    /* remaining factor (if any) */
    if (n > 1 && n % 4 != 1) return false;
    return true;
}

/* test whether a candidate k satisfies the sequence condition */
static bool good_k(uint64_t k)
{
    if (k == 1) return true;                 /* T₁ = 1 */
    if (!has_only_1mod4_factors(k)) return false;
    uint64_t half = (k + 1) / 2;              /* k ≡ 1 (mod 8) so (k+1)/2 is odd */
    return has_only_1mod4_factors(half);
}

/* add one term to the memoisation array */
static void add_term(seq_t val)
{
    if (seq_len == seq_cap)
    {
        seq_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
        seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
    }
    seq_arr[seq_len++] = val;
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n≥1 in the OEIS definition */

    init_primes();

    static uint64_t next_k = 1;    /* start at k = 1, then 9, 17, … */

    while (seq_len < (size_t)n)
    {
        if (good_k(next_k))
            add_term((seq_t)next_k);
        next_k += 8;               /* only candidates ≡ 1 (mod 8) */
    }
    return seq_arr[n - 1];
}
