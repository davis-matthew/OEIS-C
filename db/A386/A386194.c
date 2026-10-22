/*
 * A386194  Primes having only {4, 6, 7, 9} as digits.
 * https://oeis.org/A386194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 48, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
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
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

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
