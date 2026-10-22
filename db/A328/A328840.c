/*
 * A328840  Numbers with no digit 1 in their primorial base expansion
 *          (A049345).
 * https://oeis.org/A328840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 6387, u32 n <= 8488, u64 n <= 8488,
 *            u128 n <= 8488, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with no digit 1 in their primorial base expansion.
   Search algorithm: incrementally test every integer for the
   property; cache results for repeated calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Primorial bases: primes p_k are used as digit limits.
   Digits are computed as v % p_k; the value is always
   reduced by division by the same prime. */
static const unsigned int _primorial_primes[] = {
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
   919,929,937,941,947,953,967,971,977,983,991,997
};
static const size_t _primorial_primes_len = sizeof(_primorial_primes) / sizeof(_primorial_primes[0]);

/* Check whether the primorial base expansion of v contains a digit 1. */
static bool better_have_no_digit_one(seq_t v) {
    size_t pos = 0;
    while (v > (seq_t)0) {
        if (pos >= _primorial_primes_len) break;   /* safety, should not happen for our limits */
        unsigned int p = _primorial_primes[pos];
        unsigned int digit = (unsigned int)(v % (seq_t)p);
        if (digit == 1) return false;
        v /= (seq_t)p;
        ++pos;
    }
    return true;
}

/* Dynamic cache for previously computed terms. */
static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;
static size_t a_cache_cap = 0;
static seq_t a_last_value = (seq_t)0;     /* last integer inspected */
static idx_t  a_count = 0;               /* number of terms found so far */

/* Grow the cache capacity if necessary. */
static void a_cache_push(seq_t val) {
    if (a_cache_size == a_cache_cap) {
        size_t new_cap = a_cache_cap == 0 ? 64 : a_cache_cap * 2;
        seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (a_cache) {
            memcpy(newp, a_cache, a_cache_size * sizeof(seq_t));
            free(a_cache);
        }
        a_cache = newp;
        a_cache_cap = new_cap;
    }
    a_cache[a_cache_size++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) { /* offset is 1, but handle gracefully */
        return (seq_t)0;
    }

    /* Initialization on first call. */
    if (a_cache_size == 0) {
        a_cache_push((seq_t)0);   /* a(1) = 0 */
        a_last_value = (seq_t)0;
        a_count = 1;
    }

    /* Generate terms until we have at least n terms. */
    while ((idx_t)a_count < n) {
        ++a_last_value;
        if (better_have_no_digit_one(a_last_value)) {
            a_cache_push(a_last_value);
            ++a_count;
        }
    }

    return a_cache[(size_t)(n - 1)];
}
