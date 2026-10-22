/*
 * A339215  Primorial-base self numbers: numbers not of the form k +
 *          A276150(k).
 * https://oeis.org/A339215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 9615, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A339215: Primorial‑base self numbers.
   The algorithm generates all numbers of the form k + A276150(k)
   (where A276150(k) is the sum of the digits of k in primorial base)
   and collects their complements in ascending order.  The results
   are memoised so that any n ≥ 1 can be requested in any order.  */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* A list of the first two dozen primes – enough for the sizes we need. */
static const uint32_t PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};

/* ------------------------------------------------------------------ */
/* Sum of the digits of n in primorial base.                          */
/* ------------------------------------------------------------------ */
static seq_t primorial_digit_sum(idx_t n)
{
    seq_t sum = 0;
    __uint128_t prod = 1;       /* product of the primes processed so far */
    size_t i = 0;
    while (prod <= n) {
        uint32_t base = PRIMES[i];
        seq_t digit = (n / (idx_t)prod) % base;
        sum += digit;
        /* stop before prod would overflow */
        if (prod > ( (__uint128_t)n ) / base) break;
        prod *= base;
        ++i;
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/* Dynamic array of boolean flags for numbers that occur as k + sum.   */
/* ------------------------------------------------------------------ */
static unsigned char *marked = NULL;
static size_t marked_cap = 0;

static void ensure_marked_capacity(size_t needed)
{
    if (needed <= marked_cap) return;
    size_t new_cap = marked_cap ? marked_cap << 1 : 128;
    while (new_cap < needed) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(marked, new_cap);
    if (!tmp) exit(1);
    memset(tmp + marked_cap, 0, new_cap - marked_cap);
    marked = tmp;
    marked_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Cache of the self‑numbers found so far.                            */
/* ------------------------------------------------------------------ */
static seq_t *self_cache = NULL;
static size_t self_len = 0, self_cap = 0;

/* ------------------------------------------------------------------ */
/* State that is retained between calls to a(idx_t).                 */
/* ------------------------------------------------------------------ */
static idx_t next_k = 1;            /* next k that has not been processed */
static idx_t candidate = 1;         /* next number to test as a self‑number */

/* Main function for the sequence.  The index 'n' is 1‑based.          */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1 */

    /* Grow the cache as needed.  Each self number is stored once.    */
    while (self_len < (size_t)n) {
        /* Ensure we have a slot for the current candidate.           */
        if ((size_t)candidate >= marked_cap)
            ensure_marked_capacity((size_t)candidate + 1);

        /* Process all k up to (and including) the current candidate.
           Each such k may produce a marked value ≤ candidate.          */
        while (next_k <= candidate) {
            seq_t g = (seq_t)next_k + primorial_digit_sum(next_k);
            if ((size_t)g >= marked_cap)
                ensure_marked_capacity((size_t)g + 1);
            marked[(size_t)g] = 1;
            ++next_k;
        }

        /* After all relevant k have been processed, see whether
           the current candidate is marked.  If it is, increment it
           and start the outer loop again (the next candidate may
           again be marked, which will be handled in the next
           iteration).                                                   */
        if (marked[(size_t)candidate]) {
            ++candidate;
            continue;
        }

        /* The candidate is a self‑number.  Store it.                  */
        if (self_len >= self_cap) {
            size_t new_cap = self_cap ? self_cap << 1 : 16;
            seq_t *tmp = (seq_t *)realloc(self_cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            self_cache = tmp;
            self_cap = new_cap;
        }
        self_cache[self_len++] = candidate;
        ++candidate;
    }

    return self_cache[n - 1];
}
