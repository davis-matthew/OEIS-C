/*
 * A283312  a(n) = smallest missing positive number, unless a(n-1) was a prime
 *          in which case a(n) = 2*a(n-1).
 * https://oeis.org/A283312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1 * a(n) = 2*a(n-1) if a(n-1) is prime, * a(n)
 *            = smallest unused positive integer otherwise
 * Range:     u8 n <= 144, u16 n <= 34383, u32 n <= 75000, u64 n <= 75000,
 *            u128 n <= 75000, big any n
 * Validated: standard, pass, 93 terms through n = 9792 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A283312  –  lexicographically earliest permutation
 *  of the positive integers in which every prime is immediately
 *  followed by its double.  This implementation stores the
 *  sequence in a static table and memoises results so that any
 *  order of calls is supported.  The algorithm uses a bit‑set
 *  of used numbers; the size of the bit‑set grows as needed.
 *  For builds where seq_t is an arbitrary‑precision integer class
 *  (C++), casting to size_t is performed via a helper
 *  `to_sizet()` which is defined differently under C++ and C
 *  to preserve type safety.  Because this sequence grows
 *  roughly linearly, values for n ≤ 10000 stay below ~2·n;
 *  thus the bit‑set stays comfortably within ordinary addressable
 *  memory.  The algorithm runs in O(n) time with a small constant
 *  factor. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
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

/* Convert seq_t to size_t, special‑case for C++ arbitrary big ints. */
static inline size_t to_sizet(seq_t x)
{
#ifdef __cplusplus
    return static_cast<size_t>(x);
#else
    return (size_t)x;
#endif
}

/* Static storage for the sequence and the used‑number bitset. */
static seq_t *seq = NULL;        /* computed terms (0‑based)   */
static size_t seq_len = 0, seq_cap = 0;

static uint8_t *used = NULL;     /* 1 = used, 0 = unused   */
static size_t used_cap = 0;

/* Smallest unused positive integer (1‑based). */
static seq_t min_missing = (seq_t)1;

/* Grow the `used` array so that indices >= needed are valid.  The
 * array holds 0/1 flags.  Newly allocated elements are zero‑filled. */
static void ensure_used_cap(size_t needed)
{
    while (used_cap <= needed) {
        size_t new_cap = used_cap ? used_cap * 2 : 64;
        uint8_t *new_arr = (uint8_t *)realloc(used, new_cap);
        if (!new_arr) exit(1);
        memset(new_arr + used_cap, 0, new_cap - used_cap);
        used = new_arr;
        used_cap = new_cap;
    }
}

/* Grow the `seq` array to accommodate indices >= needed. */
static void ensure_seq_cap(size_t needed)
{
    while (seq_cap <= needed) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *new_arr = mem_resize(seq_t, seq, new_cap);
        if (!new_arr) exit(1);
        seq = new_arr;
        seq_cap = new_cap;
    }
}

/* Update `min_missing` after a new value `val` has been inserted.
 * This scans forward until it finds an unused integer. */
static void update_min_missing(seq_t val)
{
    if (val != min_missing) return;

    size_t idx = to_sizet(min_missing);
    ensure_used_cap(idx);
    while (idx < used_cap && used[idx]) ++idx;
    min_missing = (seq_t)idx;
    ensure_used_cap(to_sizet(min_missing));
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* safety for underflow */

    /* Initialise static storage on the first call. */
    if (!seq) {
        seq_cap = 16;
        seq = mem_new(seq_t, seq_cap);
        if (!seq) exit(1);
    }
    if (!used) {
        used_cap = 64;
        used = (uint8_t *)calloc(used_cap, 1);
        if (!used) exit(1);
    }

    /* Generate terms up to n.  We expand the array as needed. */
    while (seq_len < (size_t)n) {
        seq_t new_val;
        if (seq_len == 0) {
            new_val = (seq_t)1;                      /* a(1) = 1 */
        } else {
            seq_t prev = seq[seq_len - 1];
            if (is_prime_big(prev))                 /* prime -> double */
                new_val = prev * (seq_t)2;
            else                                    /* otherwise smallest unused */
                new_val = min_missing;
        }

        /* Mark the new value as used and update the smallest missing. */
        size_t idx = to_sizet(new_val);
        ensure_used_cap(idx);
        used[idx] = 1;
        update_min_missing(new_val);

        /* Store the new term. */
        ensure_seq_cap(seq_len);
        seq[seq_len] = new_val;
        ++seq_len;
    }

    return seq[n - 1];
}
