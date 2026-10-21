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
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 22, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, concat_parts, is_prime_big
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
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"
#include "../utils/build_primes_upto.h"

/* Helpers to cast malloc/realloc to C++ if needed. */
#ifdef __cplusplus
#define SCALLOC(ptr, sz)   (ptr)=reinterpret_cast< seq_t* >(malloc(sz))
#define SREALLOC(ptr, sz)  (ptr)=reinterpret_cast< seq_t* >(realloc(ptr, sz))
#define BOOLCALLOC(ptr, sz) (ptr)=reinterpret_cast< bool* >(malloc(sz))
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
