/*
 * A253074  Lexicographically earliest sequence of distinct numbers such that
 *          a(n-1)+a(n) is not prime.
 * https://oeis.org/A253074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9995 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Construct the lexicographically earliest permutation of
 * nonnegative integers such that for every n>1 the sum a(n-1)+a(n)
 * is not prime.  The routine lazily generates terms and memoises
 * them.  Any call to a(n) returns the correct value for all n>=1
 * irrespective of call order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

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

/* Static memoised state */
static seq_t *seq      = NULL;      /* array of sequence terms   */
static size_t seq_len  = 0;         /* number of stored terms    */
static size_t seq_cap  = 0;         /* current capacity          */

static uint8_t *used    = NULL;    /* flag: used[i]==1 if i used   */
static size_t  used_cap = 0;       /* capacity of 'used'          */

static bool initialized = false;

/* Ensure that the used array can hold at least min_cap elements. */
static void ensure_used_cap(size_t min_cap)
{
    if (min_cap <= used_cap)
        return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while (new_cap < min_cap)
        new_cap *= 2;
    uint8_t *newp = (uint8_t *)realloc(used, new_cap);
    if (!newp)
        abort();
    memset(newp + used_cap, 0, new_cap - used_cap);
    used = newp;
    used_cap = new_cap;
}

/* Ensure that the seq array can hold at least min_cap terms. */
static void ensure_seq_cap(size_t min_cap)
{
    if (min_cap <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < min_cap)
        new_cap *= 2;
    seq_t *newp = seq_realloc(seq, seq_len, new_cap);
    if (!newp)
        abort();
    seq = newp;
    seq_cap = new_cap;
}

/* Initialise the memoised state with a(1)=0. */
static void init(void)
{
    seq_cap = 16;
    seq = seq_alloc(seq_cap);
    seq_len = 0;

    used_cap = 16;
    used = (uint8_t *)calloc(used_cap, 1);
    if (!used)
        abort();

    /* a(1) = 0 */
    seq[seq_len++] = (seq_t)0;
    used[0] = 1;
    initialized = true;
}

/* Generate terms up to and including index target (1‑based). */
static void generate_up_to(idx_t target)
{
    while (seq_len < target) {
        /* Find the smallest unused integer that does not produce a prime sum. */
        idx_t candidate = 0;
        while (true) {
            if ((size_t)candidate >= used_cap)
                ensure_used_cap(candidate + 1);

            if (!used[candidate]) {
                seq_t sum = seq[seq_len - 1] + (seq_t)candidate;
                if (!is_prime_big(sum))
                    break;          /* candidate found */
            }
            ++candidate;
        }

        /* Record the chosen candidate. */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = (seq_t)candidate;

        if ((size_t)candidate >= used_cap)
            ensure_used_cap(candidate + 1);
        used[candidate] = 1;
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                          /* undefined, but never called */
    if (!initialized)
        init();

    if ((idx_t)seq_len < n)
        generate_up_to(n);
    return seq[n - 1];
}
