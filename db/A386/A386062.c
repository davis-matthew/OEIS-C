/*
 * A386062  Primes having only {0, 3, 5, 7} as digits.
 * https://oeis.org/A386062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 107, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9540 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the primes consisting only of the digits
   0, 3, 5, 7 by length‑by‑length generation. For each new requested
   index n, all missing primes are generated and cached.  The helper
   function `append_candidate` is used for dynamic array growth. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Cache of all found primes in the required order */
static seq_t *prime_cache = NULL;    /* dynamic array of primes */
static size_t prime_cache_len = 0;   /* number of stored primes      */
static size_t prime_cache_cap = 0;   /* current capacity             */
static size_t max_requested   = 0;    /* the maximum n seen so far    */
static size_t next_len        = 1;    /* length of the next batch     */

/* Allowed digits in order of increasing value */
static const seq_t DIGITS[4] = {0, 3, 5, 7};

/* Recursive generation of all numbers of a given length `len`.
   Whenever a complete number of length `len` is formed, it is tested
   for primality and appended to the cache if prime.  No early
   termination is performed – all candidates of the given length
   are examined – because future requests might need any of them. */
static void rec_gen(size_t pos, size_t len, seq_t num)
{
    if (pos == len) {
        if (is_prime_big(num))
            append_candidate(&prime_cache, &prime_cache_len, &prime_cache_cap, num);
        return;
    }
    size_t start = (pos == 0) ? 1 : 0;          /* first digit cannot be zero */
    for (size_t d = start; d < 4; ++d) {
        rec_gen(pos + 1, len, num * 10 + DIGITS[d]);
    }
}

/* Generate primes of the next length and append them to the cache. */
static void generate_len(size_t len)
{
    rec_gen(0, len, (seq_t)0);
}

/* Return the n-th prime having only the digits {0,3,5,7}.  The function
   caches all previously found primes in a static array so that repeated
   calls with arbitrary order of n remain fast. */
seq_t a(idx_t n)
{
    size_t requested = (size_t)n;           /* n is at least 1 */

    /* Generate additional primes until we have at least `requested` of them. */
    if (requested > max_requested) {
        max_requested = requested;
        while (prime_cache_len < max_requested) {
            generate_len(next_len++);
        }
    }
    return prime_cache[requested - 1];
}
