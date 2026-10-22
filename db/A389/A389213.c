/*
 * A389213  Numbers k such that A048146(k)+A162296(k) < 2*k, where A048146 is
 *          the sum of non-unitary divisors of n, and A162296 is the sum of
 *          divisors of n that have a square factor.
 * https://oeis.org/A389213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 9628, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A389213: numbers k such that A048146(k)+A162296(k) < 2*k.
   For each candidate k, compute
      sigma(k)     = sum of all divisors
      sigma_u(k)   = sum of unitary divisors = ∏ (1 + p^e)
      sigma_sf(k)  = sum of square‑free divisors = ∏ (1 + p)
   Then test 2*sigma(k) - sigma_u(k) - sigma_sf(k)  <  2*k.
   The function builds the sequence on demand and memoises all found
   terms, so repeated calls in any order are efficient. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *seq = NULL;          // memoised terms
static size_t seq_len = 0;         // number of terms stored
static size_t seq_cap = 0;         // capacity of seq array
static idx_t  checked_k = 0;       // highest k already evaluated

/* Expand the seq array when needed */
static void ensure_seq_capacity(size_t needed) {
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Test whether k satisfies the defining inequality. */
static bool satisfies_k(idx_t k) {
    uint64_t n = (uint64_t)k;            /* work on 64‑bit integer */
    seq_t sigma   = 1;                   /* product of all divisor sums   */
    seq_t sigma_u = 1;                   /* product of unitary divisor sums */
    seq_t sigma_sf= 1;                   /* product of square‑free divisor sums */
    uint64_t temp = n;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (!(temp % p)) {
            uint64_t p_pow = 1;
            uint64_t sum   = 1;          /* sum of powers of p up to e */
            uint64_t exp   = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
                p_pow *= p;              /* p^e */
                sum += p_pow;            /* add p^e to sum */
            }
            sigma     *= (seq_t)sum;
            sigma_u   *= (seq_t)(1 + p_pow);
            sigma_sf  *= (seq_t)(1 + p);
        }
    }
    if (temp > 1) {  /* remaining prime factor */
        uint64_t p = temp;
        seq_t p_pow = p;          /* p^1 */
        seq_t sum = 1 + p;        /* 1 + p */
        sigma     *= sum;
        sigma_u   *= (seq_t)(1 + p_pow);
        sigma_sf  *= (seq_t)(1 + p);
    }

    seq_t two_sigma = sigma * 2;
    seq_t test_val  = two_sigma - sigma_u - sigma_sf;
    seq_t rhs       = (seq_t)2 * (seq_t)k;
    return test_val < rhs;
}

/* Main function requested by the build environment. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, but harmless */

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        checked_k++;                     /* next candidate */
        if (satisfies_k(checked_k)) {
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)checked_k;
        }
    }
    return seq[n - 1];
}
