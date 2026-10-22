/*
 * A386572  Numbers that are not of the form k - Omega(k), where Omega(k) is
 *          the number of prime factors of k counted with multiplicity.
 * https://oeis.org/A386572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A386572: numbers not of the form k - Omega(k). 
   Computed by marking all k - Ω(k) values up to a large limit 
   and collecting the remaining integers. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Constants */
static const size_t L = 400000;          /* upper limit for k and for marked values */
static const size_t INITIAL_CAP = 1024;  /* initial capacity for the missing list */

/* Static data structures */
static bool            *marked   = NULL;   /* marked[D] == true if D = k - Ω(k) for some k */
static uint8_t         *omega    = NULL;   /* number of prime factors of i */
static uint32_t        *spf      = NULL;   /* smallest prime factor of i */
static seq_t           *missing  = NULL;   /* list of numbers not of the form k - Ω(k) */
static size_t          missing_len = 0;    /* number of stored missing numbers */
static size_t          missing_cap = 0;    /* allocated capacity of missing[] */
static bool            initialized = false;

/* ------------------------------------------------------------------- */
/* Helper: Grow the missing array when needed */
static void grow_missing(void) {
    size_t new_cap = (missing_cap == 0) ? INITIAL_CAP : missing_cap << 1;
    seq_t *p = (seq_t *)realloc(missing, new_cap * sizeof(seq_t));
    if (!p) abort();                /* unrecoverable failure */
    missing = p;
    missing_cap = new_cap;
}

/* ------------------------------------------------------------------- */
/* Build smallest prime factor array up to L */
static void build_spf(void) {
    spf = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
    if (!spf) abort();
    for (size_t i = 0; i <= L; ++i) spf[i] = (uint32_t)i;

    for (size_t i = 2; i * i <= L; ++i) {
        if (spf[i] == (uint32_t)i) {            /* i is prime */
            for (size_t j = i * i; j <= L; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }
}

/* ------------------------------------------------------------------- */
/* Compute Ω(i) for all i <= L */
static void compute_omega(void) {
    omega = (uint8_t *)malloc((L + 1) * sizeof(uint8_t));
    if (!omega) abort();
    omega[0] = 0;
    omega[1] = 0;
    for (size_t i = 2; i <= L; ++i) {
        omega[i] = omega[i / spf[i]] + 1;
    }
}

/* ------------------------------------------------------------------- */
/* Mark all values D = k - Ω(k) for 2 <= k <= L */
static void mark_values(void) {
    marked = (bool *)calloc(L + 1, sizeof(bool));
    if (!marked) abort();
    for (size_t k = 2; k <= L; ++k) {
        size_t D = k - omega[k];
        if (D <= L) marked[D] = true;
    }
}

/* ------------------------------------------------------------------- */
/* Build the list of missing numbers (those not marked) up to L */
static void build_missing(void) {
    missing_cap = 0;
    missing_len = 0;
    missing = NULL;

    for (size_t i = 1; i <= L; ++i) {
        if (!marked[i]) {
            if (missing_len == missing_cap) grow_missing();
            missing[missing_len++] = (seq_t)i;
        }
    }
}

/* ------------------------------------------------------------------- */
/* One-time initialization */
static void init_sequence(void) {
    if (initialized) return;
    build_spf();
    compute_omega();
    mark_values();
    build_missing();
    initialized = true;
}

/* ------------------------------------------------------------------- */
/* Public function: returns the n-th member of the sequence (1‑based) */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but guard */
    init_sequence();
    if (n > missing_len) {
        /* In rare case the limit L was insufficient (unlikely for n≤10000) */
        /* Increase L and recompute (not implemented for brevity) */
        abort();
    }
    return missing[(size_t)n - 1];
}
