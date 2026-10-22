/*
 * A398797  Numbers k such that psi(k) = phi(k) + tau(k)^8.
 * https://oeis.org/A398797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) are the positive integers k such that Dedekind ψ(k) = φ(k)
 *            + τ(k)^8. All known solutions are of the form k = p*(32768-p)
 *            where p and 32768-p are prime
 * Range:     u8 none, u16 none, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Comparator for qsort: sorts seq_t values in ascending order. */
static int cmp_seq(const void *a, const void *b) {
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Count of primes up to 32768 and list of products */
#define LIMIT 32768
static seq_t seq_vals[4096];
static size_t seq_len = 0;

/* Generate all k = p*(32768-p) with both p and 32768-p prime. */
static void init_seq(void) {
    static bool is_prime[LIMIT + 1];
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* Sieve of Eratosthenes up to LIMIT */
    for (int i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; (int64_t)i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* Build list of valid products */
    seq_len = 0;
    for (int p = 2; p <= LIMIT / 2; ++p) {
        if (!is_prime[p]) continue;
        int q = LIMIT - p;
        if (q < 2 || !is_prime[q]) continue;
        seq_vals[seq_len++] = (seq_t)p * (seq_t)q;
    }

    /* Sort the list */
    qsort(seq_vals, seq_len, sizeof(seq_t), cmp_seq);
}

/* Public function required by the harness */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for offset 1, but avoid underflow */
    init_seq();
    if (n - 1 < seq_len) return seq_vals[n - 1];
    return 0; /* if n beyond known terms, return 0 (undefined) */
}
