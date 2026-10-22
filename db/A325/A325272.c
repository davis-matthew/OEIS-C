/*
 * A325272  Adjusted frequency depth of n!.
 * https://oeis.org/A325272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A323014(n!) (adjusted frequency depth of n!)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses prime exponents of n! and iteratively applies
   the transform defined by A181819 until a prime is reached.
   It works for n up to at least 10 000 in well under two seconds. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* Maximum n expected in the tests */
#define MAX_N 10000
#define PRIME_LIMIT 110000  /* sufficient to get the 10 000‑th prime */

/* Prime table ----------------------------------------------------------- */
static bool primes_ready = false;
static uint32_t primes[20000];      /* primes[i] = (i+1)-th prime */
static size_t prime_cnt;

/* One‑time sieve to fill 'primes' array */
static void init_primes(void) {
    if (primes_ready) return;
    static bool is_composite[PRIME_LIMIT + 1] = {0};
    for (int i = 2; i <= PRIME_LIMIT; ++i) is_composite[i] = false;
    for (int p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (!is_composite[p]) {
            for (int q = p * p; q <= PRIME_LIMIT; q += p)
                is_composite[q] = true;
        }
    }
    prime_cnt = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = (uint32_t)i;
        }
    }
    primes_ready = true;
}

/* Compute a(n) ---------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;    /* a(1) = 0 */

    init_primes();                  /* ensure primes table is ready */

    /* Exponent frequency arrays (size up to n) */
    static idx_t *freq = NULL;
    static idx_t *freq_next = NULL;
    static idx_t freq_cap = 0;

    if ((idx_t)(n + 1) > freq_cap) {
        free(freq);
        free(freq_next);
        freq_cap = n + 1;
        freq = (idx_t *)calloc(freq_cap, sizeof(idx_t));
        freq_next = (idx_t *)calloc(freq_cap, sizeof(idx_t));
    }

    /* ----- Step 1: compute prime exponents of n! --------------------- */
    memset(freq, 0, freq_cap * sizeof(idx_t));
    idx_t curr_max_exp = 0;
    for (size_t i = 0; i < prime_cnt && primes[i] <= n; ++i) {
        idx_t p = primes[i];
        idx_t e = 0;
        idx_t div = p;
        while (div <= n) {
            e += n / div;
            div *= p;
        }
        if (e > 0) {
            freq[e] += 1;
            if (e > curr_max_exp) curr_max_exp = e;
        }
    }

    /* ----- Step 2: iterate the transform until a prime is reached ----- */
    idx_t depth = 0;
    while (true) {
        /* Check if current factorization is a single prime (exponent 1) */
        idx_t total_terms = 0;
        for (idx_t e = 1; e <= curr_max_exp; ++e) {
            if (freq[e]) {
                total_terms += freq[e];
                if (total_terms > 1) break;
            }
        }
        if (total_terms == 1 && freq[1] == 1)
            break;          /* reached a prime */

        /* Build next frequency array: frequencies of current exponents */
        idx_t new_max_exp = 0;
        for (idx_t e = 1; e <= curr_max_exp; ++e) {
            if (freq[e]) {
                idx_t v = freq[e];
                freq_next[v] += 1;
                if (v > new_max_exp) new_max_exp = v;
            }
        }

        /* Swap arrays (copy back and reset next array) */
        memcpy(freq, freq_next, (new_max_exp + 1) * sizeof(idx_t));
        memset(freq_next, 0, (new_max_exp + 1) * sizeof(idx_t));
        curr_max_exp = new_max_exp;
        depth++;
    }

    return (seq_t)(depth + 1);
}
