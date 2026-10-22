/*
 * A335738  Factorize each integer m >= 2 as the product of powers of nonunit
 *          squarefree numbers with distinct exponents that are powers of 2.
 *          The sequence lists m such that the factor with the largest
 *          exponent is a power of 2.
 * https://oeis.org/A335738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A335738: numbers m ≥ 2 that factor as a product of powers of
 * squarefree numbers with distinct exponents equal to powers of 2,
 * where the factor with the largest exponent is a power of 2.
 *
 * The code tests the condition directly from the prime‐factorisation,
 * stores found terms in a growing cache, and returns the nth term.
 * It is compatible with both C11 and C++17.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------
 *  Helper: check whether a value m satisfies the sequence definition
 *  The test works for values that fit into uint64_t; larger values
 *  are considered outside the sequence for safety.
 * ---------- */
static bool is_in_sequence(seq_t m)
{
    if (m < (seq_t)2)        /* we need m ≥ 2 */
        return false;

    /* Reject values that do not fit into a 64‑bit unsigned integer.
       The sequence for n up to about 10⁴ always lies far below this limit.   */
    seq_t max64 = (seq_t)UINT64_MAX;
    if (m > max64)
        return false;

    uint64_t N = (uint64_t)m;   /* safe cast after the check above */

    /* For each exponent bit we remember whether any odd prime used
     * this bit and whether prime 2 used this bit. */
    bool odd_at[64]  = {false};
    bool two_at[64]  = {false};
    int   high_bit   = -1;      /* greatest bit position seen */

    /* Factor out all powers of 2 first. */
    if ((N & 1ULL) == 0) {
        int e = 0;
        while ((N & 1ULL) == 0) {
            ++e;
            N >>= 1;
        }
        /* Examine bits of e. */
        uint64_t pe = (uint64_t)e;
        int k = 0;
        while (pe) {
            if (pe & 1ULL) {
                if (high_bit < k) high_bit = k;
                two_at[k] = true;
            }
            pe >>= 1;
            ++k;
        }
    }

    /* Factor odd primes. */
    for (uint64_t p = 3; p <= N / p; p += 2) {
        if (N % p == 0) {
            int e = 0;
            while (N % p == 0) {
                ++e;
                N /= p;
            }
            uint64_t pe = (uint64_t)e;
            int k = 0;
            while (pe) {
                if (pe & 1ULL) {
                    if (high_bit < k) high_bit = k;
                    odd_at[k] = true;
                }
                pe >>= 1;
                ++k;
            }
        }
    }

    /* Remaining prime factor (if any) has exponent 1 (bit 0). */
    if (N > 1) {
        if (high_bit < 0) high_bit = 0;
        odd_at[0] = true;
    }

    /* If no exponent was found a mistake occured (should not happen). */
    if (high_bit < 0)
        return false;

    /* The factor with the greatest exponent must belong to prime 2,
     * i.e. the highest bit must appear exclusively for prime 2.   */
    if (odd_at[high_bit])   return false;   /* odd prime uses the highest bit */
    if (!two_at[high_bit])  return false;   /* even prime 2 does not use it */

    return true;
}

/* ----------
 *  Cache of already generated terms.
 * ---------- */
static seq_t *cache      = NULL;    /* dynamic array of terms */
static size_t cache_size = 0;       /* number of stored terms */
static size_t cache_cap  = 0;       /* current allocation capacity */
static seq_t candidate   = 2;       /* next integer to test */

/* Grow the cache to accomodate 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    /* cast is required for C++ compatibility */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                  /* memory exhaustion: terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate terms until we have at least 'n' of them. */
static void ensure_cache(idx_t n)
{
    while (cache_size < (size_t)n) {
        if (is_in_sequence(candidate)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = candidate;
        }
        ++candidate;
    }
}

/* ----------
 *  Public entry point required by the test harness.
 * ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for offset 1 */
    ensure_cache(n);
    return cache[n - 1];
}
