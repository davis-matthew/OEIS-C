/*
 * A354711  Numbers k such that the number of divisors of k divides k-1.
 * https://oeis.org/A354711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that the number of divisors d(k) divides k-1
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Ensure this function is called with the build's seq_t and idx_t types. */

static uint64_t *spf = NULL;          /* smallest prime factor array */
static idx_t      spf_limit = 0;      /* current size of spf (max indexed value) */

static seq_t   *seq = NULL;           /* cache of sequence values a(1), a(2), ... */
static size_t   seq_size = 0;         /* number of values cached */
static size_t   seq_cap  = 0;         /* allocated capacity of seq */

static idx_t    current_k = 1;        /* next integer to test */

/* ---- Helper: rebuild spf array up to "limit" ---- */
static void rebuild_spf(idx_t limit) {
    /* Reallocate spf array */
    if (spf) {
        free(spf);
    }
    spf = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    if (!spf) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {             /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
    }
    spf_limit = limit;
}

/* ---- Helper: make sure spf array covers up to "limit" ---- */
static void ensure_spf(idx_t limit) {
    if (limit > spf_limit) {
        rebuild_spf(limit);
    }
}

/* ---- Compute the divisor count τ(k) using spf array ---- */
static seq_t divisor_count(uint64_t k) {
    seq_t res = 1;
    while (k > 1) {
        uint64_t p = spf[k];
        uint32_t exp = 0;
        while (k % p == 0) {
            k /= p;
            ++exp;
        }
        res *= (exp + 1);
    }
    return res;
}

/* ---- Ensure capacity of sequence cache ---- */
static void ensure_seq_cap(size_t needed) {
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap > 0 ? seq_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) exit(EXIT_FAILURE);
    seq_cap = new_cap;
}

/* ---- The required sequence function ---- */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n < 1, but return 0 */

    /* Generate values until we reach n */
    while (seq_size < n) {
        ensure_spf(current_k);
        seq_t d = divisor_count((uint64_t)current_k);
        if (((seq_t)(current_k - 1)) % d == 0) {
            ensure_seq_cap(seq_size + 1);
            seq[seq_size] = current_k;
            ++seq_size;
        }
        ++current_k;
    }
    return seq[n - 1];
}
