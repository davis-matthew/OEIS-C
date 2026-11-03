/*
 * A244862  List of pairs of prime numbers (p,q) starting with (2, 3) such
 *          that p || q (where || denotes concatenation) is a prime number and
 *          the sequence is always extended with the smallest prime not yet
 *          present in the sequence.
 * https://oeis.org/A244862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 91 terms through n = 987 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244862: iteratively build the list of primes
 *   p, q   where p is the smallest unused prime,
 *          q the smallest unused prime such that concat(p,q) is prime,
 *   and append p and q (if not already used) to the sequence.
 * Supports arbitrary-precision seq_t via the provided utilities. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

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

/* Helpers to cast malloc/realloc to C++ if needed. */
#ifdef __cplusplus
#define SCALLOC(ptr, sz)   (ptr)=mem_new(seq_t, (sz) / sizeof(seq_t))
#define SREALLOC(ptr, sz)  (ptr)=mem_resize(seq_t, ptr, (sz) / sizeof(seq_t))
#define BOOLCALLOC(ptr, sz) (ptr)=mem_new(bool, (sz) / sizeof(bool))
#else
#define SCALLOC(ptr, sz)   (ptr)=malloc(sz)
#define SREALLOC(ptr, sz)  (ptr)=realloc(ptr, sz)
#define BOOLCALLOC(ptr, sz) (ptr)=malloc(sz)
#endif

/* Static state. */
static seq_t *seq = NULL;      /* the sequence values  */
static size_t seq_len = 0;     /* number of elements stored */
static size_t seq_cap = 0;     /* capacity of seq array        */

static uint64_t *prime_list = NULL;   /* list of primes up to LIMIT   */
static size_t prime_count = 0;        /* number of primes in prime_list */
static size_t prime_idx = 0;          /* index of next candidate prime */

static bool *used = NULL;     /* used[p] == true if prime p is already in seq */
static const idx_t LIMIT = 2000000; /* sufficient for n <= 10000 */

static void ensure_prime_list(void)
{
    if (prime_list != NULL)
        return;                     /* already built */

    /* Build list of primes up to LIMIT. */
    build_primes_upto(LIMIT, &prime_list, &prime_count);
    if (prime_list == NULL)
        abort();

    /* Allocate usage bitmap. */
    BOOLCALLOC(used, (LIMIT + 1) * sizeof(bool));
    if (used == NULL)
        abort();
    for (size_t i = 0; i < (LIMIT + 1); ++i)
        used[i] = false;
}

static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap : 32;
    while (new_cap < need)
        new_cap <<= 1;
    SREALLOC(seq, new_cap * sizeof(seq_t));
    if (seq == NULL)
        abort();
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* safety guard – offset is 1 */
        return (seq_t)0;

    /* First call initialises the static state. */
    if (seq == NULL) {
        SCALLOC(seq, 32 * sizeof(seq_t));
        if (seq == NULL)
            abort();
        seq_len = 0;
        seq_cap = 32;
    }

    ensure_prime_list();

    /* Build terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        /* Find next unused prime p. */
        while (prime_idx < prime_count && used[prime_list[prime_idx]])
            ++prime_idx;
        if (prime_idx == prime_count)
            abort();                /* ran out of primes – should not happen */

        idx_t p = prime_list[prime_idx++];
        /* Find the smallest unused q such that concat(p,q) is prime. */
        idx_t q = 0;
        for (size_t i = 0; i < prime_count && !q; ++i) {
            if (used[prime_list[i]])
                continue;
            seq_t c = concat_parts(p, prime_list[i]);
            if (is_prime_big(c))
                q = prime_list[i];
        }
        if (!q)
            abort();                /* no suitable q found */

        /* Append p. */
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len++] = (seq_t)p;
        used[p] = true;

        /* Append q if it isn't already used. */
        if (!used[q]) {
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)q;
            used[q] = true;
        }
    }

    return seq[n - 1];
}
