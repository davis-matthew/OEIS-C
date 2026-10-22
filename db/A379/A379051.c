/*
 * A379051  Lexicographically earliest infinite sequence of distinct positive
 *          numbers with the property that n is a member of the sequence iff
 *          a(n) is composite.
 * https://oeis.org/A379051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the lexicographically earliest permutation of ℕ * such
 *            that n is a member of the sequence (i.e., appears as a value) *
 *            iff a(n) is composite. The construction below implements this *
 *            by repeatedly assigning the smallest unused admissible number
 * Range:     u8 n <= 225, u16 n <= 8358, u32 n <= 8358, u64 n <= 8358,
 *            u128 n <= 8358, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* The types seq_t and idx_t are provided by the build environment */
extern seq_t  a(idx_t n);      /* forward declaration */

/* ------------------------------------------------------------------ */
/*  Constants and static storage                                           */
/* ------------------------------------------------------------------ */
#define MAX_LIMIT 200000        /* sufficient for n up to ≈10^4 */

static bool is_prime[MAX_LIMIT + 1];
static bool is_excluded[MAX_LIMIT + 1];   /* numbers that may NOT appear as values */
static bool used_val  [MAX_LIMIT + 1];    /* numbers already used as values */
static idx_t prime_pos[MAX_LIMIT + 1];    /* 0 if non‑prime, otherwise 1‑based prime index */

static seq_t *seq_vals = NULL;           /* array of computed sequence terms */
static size_t seq_len  = 0;              /* number of terms computed so far */
static size_t seq_cap  = 0;              /* capacity of seq_vals */

/* Pointers to the next candidate prime/composite not yet used */
static idx_t next_prime   = 2;
static idx_t next_comp    = 4;

/* Flag whether the global tables have been built */
static bool tables_built = false;

/* ------------------------------------------------------------------ */
/*  Helper: build primality tables and excluded‑number lookup          */
/* ------------------------------------------------------------------ */
static void build_tables(void) {
    if (tables_built) return;
    tables_built = true;

    /* sieve of Eratosthenes */
    for (idx_t i = 0; i <= MAX_LIMIT; ++i) is_prime[i] = false;
    is_prime[2] = true;
    for (idx_t i = 3; i <= MAX_LIMIT; i += 2) is_prime[i] = true;

    for (idx_t i = 2; i * i <= MAX_LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= MAX_LIMIT; j += i) is_prime[j] = false;
        }
    }

    /* prime positions */
    idx_t pos = 0;
    for (idx_t i = 2; i <= MAX_LIMIT; ++i) {
        if (is_prime[i]) {
            ++pos;
            prime_pos[i] = pos;
        } else {
            prime_pos[i] = 0;
        }
    }

    /* excluded numbers: 1, 3, 11 and primes with even index >= 6 */
    for (idx_t i = 0; i <= MAX_LIMIT; ++i) is_excluded[i] = false;
    is_excluded[1] = is_excluded[3] = is_excluded[11] = true;
    for (idx_t i = 2; i <= MAX_LIMIT; ++i) {
        if (is_prime[i]) {
            idx_t pidx = prime_pos[i];
            if (pidx >= 6 && (pidx & 1) == 0) is_excluded[i] = true;
        }
    }

    /* initialise used table */
    for (idx_t i = 0; i <= MAX_LIMIT; ++i) used_val[i] = false;
}

/* ------------------------------------------------------------------ */
/*  Helper: advance next_prime to the next admissible unused prime     */
/* ------------------------------------------------------------------ */
static void advance_next_prime(void) {
    /* guaranteed to find within bounds for n up to 10^4 */
    while (next_prime <= MAX_LIMIT) {
        if (!used_val[next_prime] && is_prime[next_prime] && !is_excluded[next_prime]) break;
        ++next_prime;
    }
}

/* ------------------------------------------------------------------ */
/*  Helper: advance next_comp to the next admissible unused composite */
/* ------------------------------------------------------------------ */
static void advance_next_comp(void) {
    while (next_comp <= MAX_LIMIT) {
        if (!used_val[next_comp] && !is_prime[next_comp] && next_comp >= 4 && !is_excluded[next_comp]) break;
        ++next_comp;
    }
}

/* ------------------------------------------------------------------ */
/*  Ensure seq_vals has capacity for at least n terms                  */
/* ------------------------------------------------------------------ */
static void ensure_capacity(idx_t n) {
    if (seq_cap >= (size_t)n) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!seq_vals) abort();
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    build_tables();

    ensure_capacity(n);

    /* Compute missing terms up to n */
    while (seq_len < n) {
        idx_t idx = seq_len + 1;              /* current index (1‑based) */

        bool excluded = is_excluded[idx];

        seq_t value;
        if (excluded) {
            advance_next_prime();
            value = (seq_t)next_prime;
            used_val[next_prime] = true;
            ++next_prime;
        } else {
            advance_next_comp();
            value = (seq_t)next_comp;
            used_val[next_comp] = true;
            ++next_comp;
        }

        seq_vals[seq_len++] = value;
    }

    return seq_vals[n - 1];
}
