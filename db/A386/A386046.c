/*
 * A386046  Primes having only {0, 2, 3, 9} as digits.
 * https://oeis.org/A386046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 114, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9966 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function enumerates numbers whose decimal digits belong to
 * {0,2,3,9}.  Each candidate is tested for primality with
 * is_prime_big, and the prime values are cached so that a(n) is
 * returned on-demand.  It is fully compliant with the OEIS
 * indexing rule: a(1) = 2. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

/* Static data used for memoisation */
static seq_t *prime_cache = NULL;   /* cached prime values            */
static size_t prime_cap   = 0;      /* capacity of prime_cache        */
static size_t prime_count = 0;      /* number of primes cached        */
static size_t current_len = 0;      /* current digit length generated */

/* Forward declarations */
static void generate_length(size_t len);
static void gen_rec(size_t pos, size_t len, seq_t val);
static void push_prime(seq_t p);
static void ensure_primes(idx_t n);

/* Return the n-th term of the sequence (a(1) = 2).  Calls to a() may
 * arrive in arbitrary order; the function caches previously found
 * primes. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset protection */
    ensure_primes(n);              /* make sure at least n primes are cached */
    return prime_cache[n-1];       /* a(1) is at index 0 */
}

/* Make sure at least n primes are cached, extending the search as
 * required.  This function is safe for arbitrary call order. */
static void ensure_primes(idx_t n)
{
    while (prime_count < (size_t)n) {
        ++current_len;
        generate_length(current_len);   /* generate all numbers of this length */
    }
}

/* Generate all numbers of the given decimal length composed of
 * digits {0,2,3,9}.  Leading zeros are prohibited. */
static void generate_length(size_t len)
{
    gen_rec(0, len, (seq_t)0);
}

/* Depth‑first generation of a single number.  When a full number is
 * built it is tested for primality. */
static void gen_rec(size_t pos, size_t len, seq_t val)
{
    /* Allowed digits, in ascending order so that primes are found
     * in increasing order. */
    static const unsigned char dig[4] = {0, 2, 3, 9};

    if (pos == len) {
        if (is_prime_big(val))
            push_prime(val);
        return;
    }

    for (size_t i = 0; i < 4; ++i) {
        if (pos == 0 && dig[i] == 0) continue;     /* no leading zero */
        gen_rec(pos + 1, len, val * 10 + dig[i]);
    }
}

/* Store a newly found prime in the cache, resizing the array if
 * necessary. */
static void push_prime(seq_t p)
{
    if (prime_count >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
        if (!tmp) abort();                    /* allocation failure */
        prime_cache = tmp;
        prime_cap = new_cap;
    }
    prime_cache[prime_count++] = p;
}
