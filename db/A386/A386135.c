/*
 * A386135  Primes having only {1, 6, 7, 8} as digits.
 * https://oeis.org/A386135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 86, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the sequence of primes whose decimal digits belong
 * to the set {1,6,7,8}.  It does not use a closed form formula; instead it
 * performs an incremental search, generating candidates in increasing
 * numeric order and testing them for primality with the supplied
 * `isPrimeGeneric` helper.  The results are cached statically so that
 * subsequent calls with any order of indices are answered immediately. */

#include <stddef.h>                 /* for size_t */
#include "../utils/isPrimeGeneric.h"
#include "../utils/append_candidate.h"

/*------------------------------------------------------------*/
/*  Static state used by the generator and the recursive helper  */
/*------------------------------------------------------------*/
static seq_t *seq_cache = NULL;       /* dynamic array of primes found */
static size_t seq_cache_count = 0;    /* number of primes stored in seq_cache */
static size_t seq_cache_cap   = 0;    /* capacity of seq_cache buffer */
static idx_t  next_len        = 0;    /* maximum digit length processed so far */

/*------------------------------------------------------------*/
/*  Recursive routine that builds all integers with a given
 *  digit length whose digits are in {1,6,7,8} and whose last
 *  digit is either 1 or 7.  For each complete integer, the
 *  sequence's prime test is applied and successful ones are
 *  appended to the cache via append_candidate.                 */
/*------------------------------------------------------------*/
static void gen_len_rec(int pos, seq_t val, int len)
{
    /* If we have placed all digits, test primality. */
    if (pos == len) {
        if (isPrimeGeneric(val))
            append_candidate(&seq_cache, &seq_cache_count, &seq_cache_cap, val);
        return;
    }

    /* Last position – only odd digits allowed (1 or 7). */
    if (pos == len - 1) {
        static const seq_t last_digits[2] = {1, 7};
        for (int i = 0; i < 2; ++i) {
            seq_t new_val = val * 10 + last_digits[i];
            gen_len_rec(pos + 1, new_val, len);
        }
    } else {
        /* Intermediate positions may use any of the four allowed digits. */
        static const seq_t all_digits[4] = {1, 6, 7, 8};
        for (int i = 0; i < 4; ++i) {
            seq_t new_val = val * 10 + all_digits[i];
            gen_len_rec(pos + 1, new_val, len);
        }
    }
}

/*------------------------------------------------------------*/
/*  Main accessor for the OEIS sequence A386135                   */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  The behaviour for n == 0
     * is unspecified but we return 0 to avoid accessing out of bounds. */
    if (n == 0)
        return (seq_t)0;

    /* Generate primes of increasing digit lengths until we reach at least n terms. */
    while (seq_cache_count < (size_t)n) {
        ++next_len;            /* increase the processed number length */
        gen_len_rec(0, (seq_t)0, (int)next_len);
    }

    return seq_cache[n - 1];
}
