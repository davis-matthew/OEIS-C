/*
 * A386194  Primes having only {4, 6, 7, 9} as digits.
 * https://oeis.org/A386194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 86, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9527 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes whose decimal digits are only {4,6,7,9}.  
   Enumeration is performed in ascending numeric order: first all 1‑digit
   numbers 7,9, then all 2‑digit numbers (4…9)×10 + {7,9}, and so on.
   Each candidate is tested for primality with is_prime_big.
   The function returns the n‑th such prime (1‑based offset). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static const uint8_t DIGIT_MAP[4] = {4, 6, 7, 9};

/* powers of 4, used to compute how many candidates of a given length exist */
static uint64_t POW4[64];
static bool     POW4_INITIALIZED = false;

static void ensure_pow4(void)
{
    if (POW4_INITIALIZED) return;
    POW4[0] = 1ULL;
    for (int i = 1; i < 64; ++i) POW4[i] = POW4[i - 1] * 4ULL;
    POW4_INITIALIZED = true;
}

/* Construct the candidate of length `len` (len>=1) from the
   enumeration index `combo`.  For len>1, only the final digit is
   restricted to 7 or 9 (the other digits are taken from DIGIT_MAP). */
static seq_t candidate_from_state(int len, uint64_t combo)
{
    if (len == 1)
        return (seq_t)(combo == 0 ? 7 : 9);

    uint64_t prefix_idx = combo / 2;          /* 0 .. 4^(len-1)-1   */
    int       last_digit = (combo & 1) ? 9 : 7;

    /* Decode the (len‑1)-digit prefix from base‑4 representation. */
    uint8_t digits[32];                       /* enough for cur_len≤31 */
    int pos = len - 2;
    while (pos >= 0)
    {
        digits[pos]   = DIGIT_MAP[prefix_idx % 4ULL];
        prefix_idx  >>= 2;
        --pos;
    }

    /* Build the numeric value. */
    seq_t val = 0;
    for (int i = 0; i < len - 1; ++i) val = val * 10 + digits[i];
    val = val * 10 + last_digit;
    return val;
}

/* ---------- dynamic cache of found primes ---------- */
static seq_t *prime_cache      = NULL;
static size_t prime_cache_len  = 0;
static size_t prime_cache_cap  = 0;

/* Append a new prime to the cache, allocating more space if required. */
static void append_prime(seq_t val)
{
    if (prime_cache_len == prime_cache_cap)
    {
        size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 16;
        seq_t *tmp;

        if (!prime_cache)
            tmp = seq_alloc(new_cap);
        else
            tmp = seq_realloc(prime_cache, prime_cache_cap, new_cap);

        if (!tmp) exit(EXIT_FAILURE);
        prime_cache = tmp;
        prime_cache_cap = new_cap;
    }
    prime_cache[prime_cache_len++] = val;
}

/* Keep generating primes until at least `n` have been found. */
static void ensure_cache_size(idx_t n)
{
    ensure_pow4();

    static int   curr_len     = 1;
    static uint64_t curr_combo = 0;
    static uint64_t combo_limit = 2ULL;           /* 2 for length 1 (7,9) */

    while (prime_cache_len < (size_t)n)
    {
        seq_t cand = candidate_from_state(curr_len, curr_combo);
        ++curr_combo;
        if (curr_combo >= combo_limit)
        {
            ++curr_len;
            curr_combo = 0;
            if (curr_len == 1)
                combo_limit = 2ULL;
            else
                combo_limit = 2ULL * POW4[curr_len - 1];
        }

        if (is_prime_big(cand)) append_prime(cand);
    }
}

/* ---------- public entry point ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for offset 0 */
    ensure_cache_size(n);
    return prime_cache[n - 1];
}
