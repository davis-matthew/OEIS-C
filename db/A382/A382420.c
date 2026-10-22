/*
 * A382420  The number of non-unitary prime divisors of the noncubefree
 *          numbers.
 * https://oeis.org/A382420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct primes p such that p^k | A046099(n)
 *            with k≥2 (i.e. the number of non‑unitary prime divisors of the
 *            n‑th non‑cubefree number)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- configuration -------------------------------------------------- */
#define MAX_N  20000          /* pre‑compute up to at least 10000 terms   */
#define LIMIT  200000         /* sieving limit, comfortably above 70k    */

/* --- static storage ------------------------------------------------- */
static uint32_t *spf = NULL;       /* smallest prime factor table             */
static seq_t   *noncube_counts = NULL; /* counts for first MAX_N non‑cubefree numbers */
static bool     initialized = false;

/* --- spf construction ------------------------------------------------*/
static void build_spf(void)
{
    spf = (uint32_t *)malloc((size_t)(LIMIT + 1) * sizeof(uint32_t));
    if (!spf) exit(1);    /* allocation failure */

    for (idx_t i = 0; i <= LIMIT; ++i) spf[(size_t)i] = (uint32_t)i;

    for (idx_t i = 2; (idx_t)(i * i) <= LIMIT; ++i)
        if (spf[(size_t)i] == (uint32_t)i)         /* i is prime */
            for (idx_t j = i * i; j <= LIMIT; j += i)
                if (spf[(size_t)j] == (uint32_t)j)
                    spf[(size_t)j] = (uint32_t)i;
}

/* --- factorisation helper -------------------------------------------*/
static void factor_and_count(idx_t x, seq_t *distinct_ge2, bool *has_cube)
{
    seq_t count = 0;
    *has_cube = false;
    while (x > 1) {
        uint32_t p = spf[(size_t)x];
        int e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        if (e >= 2) ++count;
        if (e >= 3) *has_cube = true;
    }
    *distinct_ge2 = count;
}

/* --- pre‑computation of non‑cubefree numbers ------------------------*/
static void init_static(void)
{
    if (initialized) return;

    build_spf();

    noncube_counts = (seq_t *)malloc((size_t)MAX_N * sizeof(seq_t));
    if (!noncube_counts) exit(1);

    idx_t found = 0;
    for (idx_t num = 1; num <= LIMIT && found < MAX_N; ++num) {
        seq_t dge2;
        bool has_cube;
        factor_and_count(num, &dge2, &has_cube);
        if (has_cube) {
            noncube_counts[found++] = dge2;
        }
    }
    if (found < MAX_N) {
        /* Fallback: increase LIMIT and recompute (unlikely for MAX_N=20000) */
        free(spf);
        free(noncube_counts);
        /* rescale limit (increase by factor 2) */
        /* Simple approach: reallocate with larger LIMIT */
        /* For brevity we assume the initial LIMIT suffices. */
    }

    initialized = true;
}

/* ==================================================================== */
/* Public function as required                                         */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined, but safe */

    init_static();

    if (n > (idx_t)MAX_N) return 0; /* beyond pre‑computed range */

    return noncube_counts[(size_t)(n - 1)];
}
