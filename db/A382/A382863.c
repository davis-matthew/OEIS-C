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
 * Range:     u8 n <= 13, u16 n <= 760, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9039 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
    pair_first  = mem_resize(seq_t, pair_first, new_cap);
    pair_second = mem_resize(seq_t, pair_second, new_cap);
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
