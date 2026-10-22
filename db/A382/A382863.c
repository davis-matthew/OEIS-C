/*
 * A382863  a(2*k-1) and a(2*k) are a pair of prime numbers where 9*a(2*k-1)
 *          and 8*a(2*k) are neighboring integers.
 * https://oeis.org/A382863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   A382863 – the sequence of primes p where 9*p and 8*q are
 *            consecutive integers. For each pair (p,q) we output p at odd
 *            indices and q at even indices
 * Range:     u8 n <= 13, u16 n <= 760, u32 n <= 50, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

static seq_t *pair_first  = NULL;
static seq_t *pair_second = NULL;
static size_t pair_cnt    = 0;   /* number of pairs stored          */
static size_t pair_cap    = 0;   /* capacity of the pair arrays     */
static uint64_t last_prime = 0;  /* last prime processed (0 → start) */

/* Ensure that the pair arrays can hold at least `need` elements. */
static void ensure_capacity(size_t need)
{
    if (need <= pair_cap) return;
    size_t new_cap = pair_cap == 0 ? 64 : pair_cap * 2;
    while (new_cap < need) new_cap <<= 1;
    pair_first  = (seq_t *)realloc(pair_first,  new_cap * sizeof(seq_t));
    pair_second = (seq_t *)realloc(pair_second, new_cap * sizeof(seq_t));
    pair_cap    = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                           /* out of range */

    /* Number of pairs that must be available. */
    size_t needed_pairs = (n + 1) / 2;

    /* Generate pairs until we have enough. */
    while (pair_cnt < needed_pairs) {
        /* Get next prime. */
        uint64_t p;
        if (last_prime == 0) {
            p = 2;
        } else {
            p = next_prime(last_prime + 1);
        }
        last_prime = p;

        /* Only primes ≡ 1 or 7 (mod 8) can produce an integer q. */
        if ((p & 7ULL) != 1ULL && (p & 7ULL) != 7ULL)
            continue;

        unsigned __int128 cand;
        if ((p & 7ULL) == 1ULL)          /* 9p - 1 divisible by 8 */
            cand = (unsigned __int128)9 * p - 1;
        else                              /* 9p + 1 divisible by 8 */
            cand = (unsigned __int128)9 * p + 1;

        /* cand must be divisible by 8 (always true by construction). */
        uint64_t q = (uint64_t)(cand >> 3);   /* divide by 8 */

        if (is_prime_big((seq_t)q)) {
            ensure_capacity(pair_cnt + 1);
            pair_first[pair_cnt]  = (seq_t)p;
            pair_second[pair_cnt] = (seq_t)q;
            ++pair_cnt;
        }
    }

    /* Return the requested term. */
    if (n & 1ULL)   /* odd index → first element of the pair */
        return pair_first[(n - 1) / 2];
    else            /* even index → second element of the pair */
        return pair_second[(n / 2) - 1];
}
