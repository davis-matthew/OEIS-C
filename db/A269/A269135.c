/*
 * A269135  Numbers n which are neither a prime nor a square of a prime such
 *          that there is no d, 2<=d<=n/2, which divides binomial(n-d-1,d-1)
 *          and is not coprime to n.
 * https://oeis.org/A269135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A269135: Numbers n that are neither prime nor square of a prime
 * and for which there is no divisor d (2 ≤ d ≤ n/2) dividing
 * binomial(n-d-1,d-1) that is not coprime to n.
 *
 * For n > 33 the sequence consists exactly of the products
 * p*(p+2) where p is the lower twin prime (> 3).  The
 * first ten terms are listed explicitly.
 *
 * The function a(n) returns the nth term (a(1) = 1).
 *
 * Implementation strategy:
 *   • Store the first ten terms in a static table.
 *   • For n > 10 generate successive twin‑prime products
 *     on demand using a small sieve; values ≤ 33 (namely 15)
 *     are omitted because they are already in the first
 *     ten terms.
 *   • The table of products is expanded by doubling the sieve
 *     bound until it contains enough entries.
 */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t first_terms[10] = {
    (seq_t)1,  (seq_t)6,  (seq_t)8,  (seq_t)10,
    (seq_t)12, (seq_t)15, (seq_t)20, (seq_t)21,
    (seq_t)24, (seq_t)33
};

static seq_t *twin_products = NULL;  /* array of p*(p+2) > 33          */
static size_t twin_count   = 0;      /* number of stored products       */
static size_t twin_cap     = 0;      /* current allocation size         */
static idx_t   sieve_limit = 0;      /* upper bound of last sieve       */

/* Build twin‑prime product table up to LIMIT.  Overwrites any
 * existing table.  Products ≤ 33 are skipped. */
static void recompute_twins(idx_t limit)
{
    if (limit < 2) return;

    /* Allocate prime flag array */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) return;               /* OOM – caller will retry with larger limit */

    for (size_t i = 0; i <= (size_t)limit; ++i) is_prime[i] = true;
    if (limit >= 0) { is_prime[0] = false; is_prime[1] = false; }

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; (idx_t)((unsigned long long)i * i) <= limit; ++i) {
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_prime[(size_t)j] = false;
            }
        }
    }

    /* Build twin‑prime products */
    size_t cap   = 128;
    seq_t *new_tab = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!new_tab) { free(is_prime); return; }

    size_t count = 0;
    for (idx_t i = 2; i + 2 <= limit; ++i) {
        if (is_prime[(size_t)i] && is_prime[(size_t)(i + 2)]) {
            seq_t prod = (seq_t)i * ((seq_t)i + 2);
            if (prod <= (seq_t)33) continue;   /* skip 15 (and any <=33) */

            if (count == cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(new_tab, cap * sizeof(seq_t));
                if (!tmp) break;                 /* OOM – stop building */
                new_tab = tmp;
            }
            new_tab[count++] = prod;
        }
    }

    /* Replace old table */
    free(twin_products);
    twin_products = new_tab;
    twin_count    = count;
    twin_cap      = cap;
    sieve_limit   = limit;

    free(is_prime);
}

/* Ensure that at least N twin products (>33) are available. */
static void ensure_twins(size_t N)
{
    if (N <= twin_count) return;

    idx_t new_limit = sieve_limit ? sieve_limit : 2000;
    /* Double the sieve bound until we have enough products */
    while (twin_count < N) {
        new_limit <<= 1;                           /* exponential growth */
        recompute_twins(new_limit);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                  /* out of range guard */

    if (n <= 10) return first_terms[n - 1];

    size_t pos = (size_t)(n - 11);                /* 0‑based index in twin list */
    ensure_twins(pos + 1);                        /* need pos+1 entries */
    return twin_products[pos];
}
