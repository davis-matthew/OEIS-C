/*
 * A379226  Numbers k for which A113177(k) is a multiple of 5, where A113177
 *          is fully additive with a(p) = Fibonacci(p).
 * https://oeis.org/A379226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 9726, u32 n <= 9726, u64 n <= 9726,
 *            u128 n <= 9726, big any n
 * Helpers:   build_spf_arr, fib_mod5_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A379226: a(n) is the n‑th integer k (k≥1) for which
   Σ_{p^e||k} e · Fibonacci(p) ≡ 0 (mod 5).  The sequence is
   generated on demand; the first term is k = 1. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_spf_arr.h"
#include "../utils/fib_mod5_int.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Dynamic array helpers (for seq_t). */
static void ensure_seq_capacity(size_t min_cap, seq_t **arr, size_t *cap)
{
    if (*cap < min_cap) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Persistent state across calls. */
    static seq_t *seq   = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static idx_t *spf = NULL;
    static idx_t spf_limit = 0;
    static idx_t current_k = 1;      /* next integer to test */

    /* First call: initialise structures. */
    if (seq_len == 0) {
        seq_cap = 128;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) exit(1);
        seq[0] = (seq_t)1;          /* a(1) = 1 (A113177(1)=0) */
        seq_len = 1;

        spf_limit = 2;
        spf = build_spf_arr(spf_limit);
        current_k = 2;
    }

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        /* Grow SPF array if needed. */
        if (current_k > spf_limit) {
            idx_t new_limit = spf_limit * 2 + 100;  /* safety margin */
            if (new_limit <= spf_limit) new_limit = spf_limit + 100;
            idx_t *new_spf = build_spf_arr(new_limit);
            free(spf);
            spf = new_spf;
            spf_limit = new_limit;
        }

        idx_t k = current_k++;
        unsigned int sum_mod5 = 0;
        idx_t temp = k;

        /* Factorise k using the SPF array. */
        while (temp > 1) {
            idx_t p = spf[temp];
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }

            /* Fibonacci(5) ≡ 0 (mod 5), so skip p==5. */
            if (p != 5) {
                unsigned int fib = fib_mod5_int(p);
                unsigned int add = ((exp % 5) * fib) % 5;
                sum_mod5 = (sum_mod5 + add) % 5;
            }
        }

        if (sum_mod5 == 0) {
            ensure_seq_capacity(seq_len + 1, &seq, &seq_cap);
            seq[seq_len++] = (seq_t)k;
        }
    }

    return seq[n - 1];
}
