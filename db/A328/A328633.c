/*
 * A328633  Numbers n for which A328578(n) = A257993(A276086(A276086(n))) = 3,
 *          where A276086 converts the primorial base expansion of n into its
 *          prime product form, and A257993 returns the index of the least
 *          prime not present in its argument.
 * https://oeis.org/A328633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 5453, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   powmod30
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328633:
   Numbers n for which A276086(A276086(n)) is a multiple of 6 but not of 5.
   Equivalent: A257993(A276086(A276086(n))) = 3. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/powmod30.h"

/* First 20 primes – enough for any 64‑bit primorial base number. */
static const uint32_t primes[20] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};

/* Return true iff n satisfies the definition of the sequence. */
static bool is_good(uint64_t n) {
    /* Compute m = A276086(n) modulo 30. */
    uint32_t m_mod30 = 1;
    uint64_t temp = n;
    for (size_t i = 0; temp && i < sizeof(primes)/sizeof(primes[0]); ++i) {
        uint32_t p = primes[i];
        uint32_t digit = temp % p;
        temp /= p;
        if (digit)
            m_mod30 = (m_mod30 * powmod30(p % 30, digit)) % 30;
    }
    /* If temp > 0 then n requires more primes – this cannot happen for 64‑bit n. */

    /* Derive the first three digits of the primorial expansion of m. */
    uint32_t g0 = m_mod30 % 2;                          /* digit for prime 2   */
    uint32_t m_div2 = (m_mod30 - g0) / 2;               /* integer division   */
    uint32_t g1 = m_div2 % 3;                          /* digit for prime 3   */
    uint32_t m_div2_div3 = (m_div2 - g1) / 3;           /* integer division   */
    uint32_t g2 = m_div2_div3 % 5;                     /* digit for prime 5   */

    /* Conditions: divisible by 2 and 3 but not by 5. */
    return (g0 == 1) && (g1 != 0) && (g2 == 0);
}

/* Cache of computed sequence values. */
static seq_t *seq_vals = NULL;
static size_t seq_len = 0;   /* number of stored terms      */
static size_t seq_cap = 0;   /* capacity of the array       */
static uint64_t last_candidate = 1;   /* last tested integer */

/* Ensure the cache has room for at least 'needed' elements. */
static void ensure_seq_capacity(size_t needed) {
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_arr = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    seq_vals = new_arr;
    seq_cap = new_cap;
}

/* Return the nth term of the sequence (1‑based).  */
seq_t a(idx_t n) {
    if (n <= seq_len) return seq_vals[n - 1];

    /* Generate terms until we reach the requested index. */
    while (seq_len < (size_t)n) {
        ++last_candidate;                     /* next integer to test   */
        if (is_good(last_candidate)) {
            ensure_seq_capacity(seq_len + 1);
            seq_vals[seq_len++] = (seq_t)last_candidate;
        }
    }
    return seq_vals[n - 1];
}
