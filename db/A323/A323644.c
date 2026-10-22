/*
 * A323644  Numbers with 3 or 4 divisors.
 * https://oeis.org/A323644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th smallest integer that has
   exactly 3 or 4 divisors.  Numbers with 3 or 4 divisors are
   of the form p², p³, or p·q with p < q prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static seq_t *seq = NULL;          /* cached sorted sequence   */
static size_t seq_len = 0;         /* number of elements cached */
static size_t seq_cap = 0;         /* capacity of seq array    */
static idx_t current_val = 4;     /* next integer to test     */

static uint64_t *prime_array = NULL;
static size_t prime_cnt = 0;

/* Compute the number of divisors of x (up to the point where
   it is known to exceed 4). */
static inline int divisor_count_ul(uint64_t x)
{
    int cnt = 1;
    for (size_t i = 0; i < prime_cnt && prime_array[i] * prime_array[i] <= x; ++i) {
        uint64_t p = prime_array[i];
        if (x % p != 0) continue;
        int exp = 0;
        do {
            x /= p;
            ++exp;
        } while (x % p == 0);
        cnt *= (exp + 1);
        if (cnt > 4) return cnt;         /* early exit */
    }
    if (x > 1) {                          /* remaining prime factor */
        cnt <<= 1;                         /* multiply by 2 */
    }
    return cnt;
}

/* Ensure that the global array 'seq' contains at least n elements.
   The array is built incrementally, so subsequent calls are fast. */
static void ensure_sequence(size_t n)
{
    if (seq && seq_len >= n) return;      /* already sufficient */

    /* Initialise resources on first call */
    if (seq == NULL) {
        const idx_t PRIME_LIMIT = 500000;  /* enough for thousands of terms */
        build_primes_upto(PRIME_LIMIT, &prime_array, &prime_cnt);

        seq_cap = 1024;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        current_val = 4;                  /* smallest candidate */
    }

    /* Extend the sequence until it has n elements */
    while (seq_len < n) {
        /* Determine divisor count of current_val */
        int dcnt = divisor_count_ul((uint64_t)current_val);
        if (dcnt == 3 || dcnt == 4) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)current_val;
        }
        ++current_val;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but harmless */
    ensure_sequence((size_t)n);
    return seq[n - 1];
}
