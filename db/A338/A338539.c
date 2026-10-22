/*
 * A338539  Numbers having exactly two non-unitary prime factors.
 * https://oeis.org/A338539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 3502, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having exactly two non‑unitary prime factors */
/* a(n) is the n‑th smallest integer k such that exactly two distinct primes p with
 * exponent ≥ 2 divide k (i.e. k is divisible by the squares of exactly two
 * distinct primes). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- configuration ----------
 * The following limit is enough to generate the first 20000 terms.
 * It is doubled automatically if more terms are needed. ----------
 */
#define INITIAL_SPFSIEVE_LIMIT 2000000ULL

/* ---------- static data ----------
 * All data are lazily initialized and preserved between calls. ----------
 */
static idx_t *spf = NULL;         /* smallest prime factor array */
static uint64_t spf_limit = 0;    /* current size of the SPF array (0 ⇒ not built) */

static seq_t *seq = NULL;         /* array of found terms */
static size_t seq_len = 0;        /* number of terms stored */
static size_t seq_cap = 0;        /* allocated capacity for seq */

/* next_candidate holds the last integer that was examined by the scanner. */
static uint64_t next_candidate = 1ULL;

/* ---------- utility: grow seq array ----------
 */
static void grow_seq(size_t new_cap) {
    if (new_cap <= seq_cap) return;
    if (seq_cap == 0) seq_cap = 16;
    while (seq_cap < new_cap) seq_cap <<= 1;
    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    if (!seq) abort();
}

/* ---------- build SPF up to 'new_limit' ----------
 */
static void build_spf(uint64_t new_limit) {
    free(spf);
    spf_limit = new_limit;
    spf = (idx_t *)calloc(spf_limit + 1, sizeof(idx_t));
    if (!spf) abort();

    /* simple linear sieve for SPF */
    idx_t *primes = (idx_t *)malloc((new_limit / 10 + 1) * sizeof(idx_t));
    size_t pcnt = 0;
    for (idx_t i = 2; i <= (idx_t)new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[pcnt++] = i;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            idx_t p = primes[j];
            if (p > spf[i] || (idx_t)i * p > new_limit) break;
            spf[(idx_t)i * p] = p;
        }
    }
    free(primes);
}

/* ---------- test whether n has exactly two primes with exponent >= 2 ----------
 */
static bool has_two_nonunitary_primes(uint64_t n) {
    int count = 0;
    while (n > 1) {
        idx_t p = spf[(idx_t)n];
        int exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        if (exp >= 2) {
            ++count;
            if (count > 2) return false;
        }
    }
    return count == 2;
}

/* ---------- generate terms until at least 'need' terms are stored ----------
 */
static void ensure_terms_up_to(size_t need) {
    /* Build SPF table once (or enlarge it later). */
    if (!spf) build_spf(INITIAL_SPFSIEVE_LIMIT);

    while (seq_len < need) {
        /* Grow the SPF sieve if we have reached its limit. */
        if (next_candidate >= spf_limit) {
            uint64_t new_limit = spf_limit * 2;
            build_spf(new_limit);
            spf_limit = new_limit;
        }

        ++next_candidate;                           /* examine next integer */
        if (has_two_nonunitary_primes(next_candidate)) {
            grow_seq(seq_len + 1);
            seq[seq_len++] = (seq_t)next_candidate;
        }
    }
}

/* ---------- public function ----------
 * a(idx_t n) returns the n-th term (n >= 1). ----------
 */
seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* safety; sequence starts at index 1 */
    ensure_terms_up_to((size_t)n);
    return seq[(size_t)n - 1];
}
