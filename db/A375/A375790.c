/*
 * A375790  Numbers k such that (sigma(k) - k)^(sigma(k) - k) == k (mod
 *          sigma(k)), where sigma = A000203.
 * https://oeis.org/A375790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer k such that (sigma(k)-k)^(sigma(k)-k) ≡
 *            k (mod sigma(k)), where sigma = A000203
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 26, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/sigma_of.h"          /* seq_t sigma_of(idx_t n); */

/* ---------- Static cache for computed sequence values ---------- */
static seq_t *seq_values = NULL;      /* array of found terms */
static size_t seq_cap = 0;           /* capacity of seq_values */
static size_t seq_len = 0;           /* number of terms stored so far */
static idx_t  next_candidate = 1;    /* next k to test */

/* ---------- Ensure enough capacity for 'needed' terms ---------- */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_values, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* out of memory */
    seq_values = tmp;
    seq_cap = new_cap;
}

/* ---------- Modular exponentiation for seq_t base, uint64_t exp, seq_t mod ---------- */
static seq_t mod_pow_big(seq_t base, uint64_t exp, seq_t mod)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1u) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1u;
    }
    return result;
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not part of the sequence (offset 1) */

    /* Return cached value if already computed */
    if ((size_t)n <= seq_len) return seq_values[n - 1];

    /* Generate terms until the requested index is reached */
    while (seq_len < n) {
        idx_t k = next_candidate++;
        if (k == 1) {                     /* k = 1 is a term by definition */
            ensure_seq_capacity(seq_len + 1);
            seq_values[seq_len++] = (seq_t)1;
            continue;
        }

        seq_t sigma = sigma_of(k);        /* σ(k) */
        seq_t d = sigma - (seq_t)k;       /* σ(k) – k (positive for k > 1) */

        /* Convert d to uint64_t.  For the ranges we target, d fits in 64 bits. */
        uint64_t exp = (uint64_t)d;

        seq_t pow_val = mod_pow_big(d, exp, sigma);

        if (pow_val == (seq_t)k) {        /* (σ(k)-k)^(σ(k)-k) ≡ k (mod σ(k)) */
            ensure_seq_capacity(seq_len + 1);
            seq_values[seq_len++] = (seq_t)k;
        }
    }

    return seq_values[n - 1];
}
