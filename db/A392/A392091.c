/*
 * A392091  Triangle read by rows: row n gives T(n,k) = Sum_{j=1..n} #{m <= j
 *          : A001222(m)=k} for k=1..floor(log_2(n)).
 * https://oeis.org/A392091
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened triangle read by rows where T(n,k) =
 *            sum_{m=2..n, Ω(m)=k} (n-m+1). Here Ω(m) is the number of prime
 *            factors of m counted with multiplicity (A001222). The values of
 *            a(n) are produced by appending each row T(n,1..⌊log2 n⌋) to a
 *            global array, with offset 2 (i.e. a(2) is the fir...
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*====================================================================*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* helper: floor(log2) for idx_t (non‑zero) */
static inline int floor_log2_idx(idx_t v)
{
    /* idx_t may be 64‑bit */
    return v == 0 ? -1 : (sizeof(idx_t)*8 - 1 - (int)__builtin_clzll(v));
}

/* Global state retained between calls */
static idx_t current_n      = 1;      /* highest n processed (>=1) */
static idx_t max_omega_seen = 0;      /* max Ω encountered so far */

/* Arrays of size max_omega_seen+1 (indices 0..max_omega_seen) */
static idx_t  *count_arr = NULL;      /* count of numbers with Ω = k */
static seq_t *sum_arr   = NULL;      /* sum of those numbers */

/* Smallest prime factor sieve */
static uint32_t *spf      = NULL;     /* spf[i] = smallest prime divide i, 0 if unavailable */
static size_t   spf_cap   = 0;        /* allocated size of spf */

/* Flattened triangle sequencing */
static seq_t  *flat  = NULL;          /* flattened entries a(2),a(3),... */
static size_t  flat_len = 0;
static size_t  flat_cap = 0;

/* Ensure array of counts and sums large enough for index k */
static void ensure_omega_capacity(idx_t k)
{
    if (k <= max_omega_seen) return;
    /* grow until large enough */
    size_t new_cap = max_omega_seen ? max_omega_seen << 1 : 8;
    while (k > (idx_t)new_cap) new_cap <<= 1;

    /* reallocate count_arr */
    idx_t *new_count = NULL;
    if (count_arr) {
        new_count = (idx_t*)realloc(count_arr, new_cap * sizeof(idx_t));
    } else {
        new_count = (idx_t*)calloc(new_cap, sizeof(idx_t));
    }
    if (!new_count) { abort(); }
    count_arr = new_count;

    /* reallocate sum_arr */
    seq_t *new_sum = NULL;
    if (sum_arr) {
        new_sum = (seq_t*)realloc(sum_arr, new_cap * sizeof(seq_t));
    } else {
        new_sum = (seq_t*)calloc(new_cap, sizeof(seq_t));
    }
    if (!new_sum) { abort(); }
    sum_arr = new_sum;

    /* initialise new slots to zero (calloc ensures) */
    max_omega_seen = (idx_t)(new_cap - 1);
}

/* Recompute smallest prime factor sieve up to 'n' (inclusive) */
static void recompute_spf(idx_t n)
{
    if (n < 2) return;
    spf_cap = (size_t)n + 1;
    spf = (uint32_t*)realloc(spf, spf_cap * sizeof(uint32_t));
    if (!spf) { abort(); }

    /* zero initialise */
    for (size_t i = 0; i < spf_cap; ++i) spf[i] = 0;
    spf[0] = 0;
    if (spf_cap > 1) spf[1] = 1;

    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i <= n / i) { /* avoid overflow */
                for (idx_t j = i * i; j <= n; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }
}

/* Compute Ω(n) using the current spf table */
static idx_t omega_val(idx_t n)
{
    idx_t cnt = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        if (p == 0) p = (uint32_t)n; /* n is prime (>n) but shouldn't happen */
        while (n % p == 0) {
            ++cnt;
            n /= p;
        }
    }
    return cnt;
}

/* Extend computed data up to index 'n' (assumes n >= current_n) */
static void extend_to(idx_t n)
{
    if (n <= current_n) return;

    if (n >= spf_cap) recompute_spf(n);

    /* Process each new integer */
    for (idx_t m = current_n + 1; m <= n; ++m) {
        idx_t omega = omega_val(m);
        if (omega > max_omega_seen) ensure_omega_capacity(omega);

        /* update counts and sums */
        ++count_arr[omega];
        sum_arr[omega] += (seq_t)m;

        /* append row m's entries to flattened array */
        int L = floor_log2_idx(m);
        if (L < 1) continue; /* m=1 not used, but guard */

        for (int k = 1; k <= L; ++k) {
            if (flat_len == flat_cap) {
                flat_cap = flat_cap ? flat_cap << 1 : 16;
                flat = (seq_t*)realloc(flat, flat_cap * sizeof(seq_t));
                if (!flat) { abort(); }
            }
            seq_t term = ((seq_t)(m + 1)) * count_arr[k] - sum_arr[k];
            flat[flat_len++] = term;
        }
    }
    current_n = n;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset is 2, lower values undefined */

    extend_to(n);                  /* guarantees flat is up to a(n) */

    size_t idx = (size_t)(n - 2);  /* zero‑based index in flat array */
    if (idx >= flat_len) {         /* safety: should not happen */
        return 0;
    }
    return flat[idx];
}
