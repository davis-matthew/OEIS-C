/*
 * A349236  Gaps between cubefree numbers: a(n) = A004709(n+1) - A004709(n).
 * https://oeis.org/A349236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A004709(n+1) - A004709(n) (gaps between successive
 *            cubefree numbers)
 * Range:     u8 n <= 3413, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- utility: grow dynamic array ----- */
static void grow_if_needed(void **ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0 ? 8 : (*cap << 1));
    while (new_cap < needed) new_cap <<= 1;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

/* ---------- prime table generation ---- */
static uint64_t *primes = NULL;
static size_t primes_sz  = 0;
static size_t primes_cap = 0;

static void add_prime(uint64_t p)
{
    grow_if_needed((void **)&primes, &primes_cap, primes_sz + 1, sizeof(uint64_t));
    primes[primes_sz++] = p;
}

static void add_primes_up_to(uint64_t limit)
{
    if (primes_sz == 0) add_prime(2);
    if (primes[primes_sz - 1] >= limit) return;

    uint64_t cand = primes[primes_sz - 1] + 1;
    if (cand <= 2) cand = 3;
    if ((cand & 1) == 0) cand++;

    for (; cand <= limit; cand += 2) {
        bool isprime = true;
        for (size_t i = 0; i < primes_sz; ++i) {
            uint64_t p = primes[i];
            __uint128_t pp = (__uint128_t)p * p;
            if (pp > cand) break;
            if (cand % p == 0) { isprime = false; break; }
        }
        if (isprime) add_prime(cand);
    }
}

/* ---------- integer cube root (floor) ---- */
static uint64_t cube_root_floor(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        __uint128_t val = (__uint128_t)mid * mid * mid;
        if (val <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* ---------- cubefree test ---- */
static bool is_cubefree_candidate(uint64_t x)
{
    uint64_t root = cube_root_floor(x);
    add_primes_up_to(root);

    for (size_t i = 0; i < primes_sz; ++i) {
        uint64_t p = primes[i];
        __uint128_t cube128 = (__uint128_t)p * p * p;
        if (cube128 > x) break;
        uint64_t cube = (uint64_t)cube128;
        if (x % cube == 0) return false;
    }
    return true;
}

/* ---------- sequence storage ----- */
static seq_t *diff = NULL;
static size_t diff_sz  = 0;
static size_t diff_cap = 0;
static seq_t last_cubefree = 0;
static bool initialized = false;

/* ---------- public function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1, n>0 only */
    if (!initialized) {                       /* first call initialises state */
        last_cubefree = 1;                    /* A004709(1) */
        initialized = true;
    }

    /* Ensure we have computed at least n gaps */
    while (diff_sz < (size_t)n) {
        uint64_t candidate = (uint64_t)last_cubefree + 1;
        while (!is_cubefree_candidate(candidate)) ++candidate;

        seq_t new_val = (seq_t)candidate;
        seq_t gap = new_val - last_cubefree;

        grow_if_needed((void **)&diff, &diff_cap, diff_sz + 1, sizeof(seq_t));
        diff[diff_sz++] = gap;

        last_cubefree = new_val;
    }

    return diff[(size_t)n - 1];
}
