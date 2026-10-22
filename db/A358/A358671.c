/*
 * A358671  Numbers k such that for all factorizations of k as x*y, the sum
 *          x+y is carryfree when the addition is done in the primorial base,
 *          A049345.
 * https://oeis.org/A358671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A358671: numbers for which every factorization x*y has x+y carry‑free in primorial base.
   Search method: trial‑division of candidates and testing all divisors. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* 200 primes – sufficient for all primorial carries of the tested terms. */
static const uint32_t primes[200] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,
    157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,
    239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,
    331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,
    421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,
    509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,
    613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,
    709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,
    821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,
    919,929,937,941,947,953,967,971,977,983,991,997,1009,1013,
    1019,1021,1031,1033,1039,1049,1051,1061,1063,1069,1087,1091,
    1093,1097,1103,1109,1117,1123,1129,1151,1153,1163,1171,1181,
    1187,1193,1201,1213,1217,1223
};

/* Return true iff addition of x and y is carry‑free in primorial base. */
static bool carryfree(seq_t x, seq_t y)
{
    size_t pos = 0;
    while (x != 0 || y != 0) {
        uint32_t base = primes[pos];
        seq_t bx = x % (seq_t)base;
        seq_t by = y % (seq_t)base;
        if (bx + by >= (seq_t)base)
            return false;
        x /= (seq_t)base;
        y /= (seq_t)base;
        ++pos;
    }
    return true;
}

/* Return true iff all factorizations of k satisfy carry‑free addition. */
static bool passes_k(seq_t k)
{
    /* k must be even (factorization 1*k). */
    if ((k & (seq_t)1) != (seq_t)0)
        return false;

    for (seq_t d = (seq_t)1; d * d <= k; ++d) {
        if (k % d != (seq_t)0)
            continue;
        seq_t e = k / d;
        if (!carryfree(d, e))
            return false;
    }
    return true;
}

/* Cache the computed sequence values. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t next_candidate = 2;
static bool seq_initialized = false;

static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Main entry: returns the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (!seq_initialized) {
        cache      = (seq_t *)malloc(64 * sizeof(seq_t));
        cache_cap  = 64;
        cache_len  = 0;
        next_candidate = 2;
        seq_initialized = true;
    }

    while (cache_len < n) {
        /* Skip odd candidates – they cannot belong to the sequence. */
        while ((next_candidate & (seq_t)1) != (seq_t)0)
            ++next_candidate;

        if (passes_k(next_candidate)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
