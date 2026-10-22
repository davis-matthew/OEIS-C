/*
 * A389636  Nondeficient numbers k such that k/p is nondeficient (see A023196)
 *          for all primes p dividing k.
 * https://oeis.org/A389636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 4823, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Nondeficient numbers k such that k/p is nondeficient for all primes p
 * dividing k (OEIS A389636).  The implementation performs a
 * brute‑force search over the integers, testing each candidate for
 * the required property.  All arithmetic is carried out with 128‑bit
 * integers to avoid overflow while keeping the code short and
 * compatible with both the 64‑bit and arbitrary‑precision
 * seq_t builds.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper: factorize a 64‑bit unsigned integer into primes and
   exponents.  The arrays pfs[] and exps[] are assumed large enough.
   Returns the number of distinct prime factors in *cnt.  */
static void factorize_u64(uint64_t n, uint64_t *pfs, int *exps, size_t *cnt)
{
    size_t c = 0;
    if (n % 2ULL == 0ULL) {
        int e = 0;
        while ((n % 2ULL) == 0ULL) {
            n /= 2ULL;
            ++e;
        }
        pfs[c] = 2ULL;
        exps[c] = e;
        ++c;
    }
    for (uint64_t p = 3ULL; p <= n / p; p += 2ULL) {
        if (n % p == 0ULL) {
            int e = 0;
            while (n % p == 0ULL) {
                n /= p;
                ++e;
            }
            pfs[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (n > 1ULL) {
        pfs[c] = n;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Test whether the integer 'cand_val' (a 64‑bit integer) satisfies
   the A389636 property.  The function returns true if it does,
   otherwise false. */
static bool is_candidate(uint64_t cand_val)
{
    /* Factorize candidate.  We expect at most 16 distinct primes. */
    uint64_t pfs[16];
    int exps[16];
    size_t cnt = 0;
    factorize_u64(cand_val, pfs, exps, &cnt);

    /* Compute sigma(k) as a 128‑bit integer.  sigma(k) is the
       product over the prime power sums. */
    unsigned __int128 sigma_k = 1;

    /* For each prime, store the sigma_part for k and for k/p. */
    unsigned __int128 sigma_part[16];
    unsigned __int128 sigma_part_prime[16];

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pfs[i];
        int e = exps[i];

        /* Compute p^(e+1) */
        unsigned __int128 p_pow = 1;
        for (int j = 0; j < e + 1; ++j) p_pow *= p;
        unsigned __int128 num = p_pow - 1;
        unsigned __int128 denom = p - 1;
        sigma_part[i] = num / denom;

        /* Compute p^e */
        p_pow = 1;
        for (int j = 0; j < e; ++j) p_pow *= p;
        num = p_pow - 1;
        sigma_part_prime[i] = num / denom;

        sigma_k *= sigma_part[i];
    }

    /* k must be non‑deficient: sigma(k) >= 2*k */
    if (sigma_k < (unsigned __int128)2 * cand_val) return false;

    /* Check every k/p */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pfs[i];
        unsigned __int128 sigma_k_div_p =
            (sigma_k / sigma_part[i]) * sigma_part_prime[i];
        unsigned __int128 two_k_over_p =
            (unsigned __int128)2 * (cand_val / p);
        if (sigma_k_div_p < two_k_over_p) return false;
    }

    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static state to memoize previously found terms. */
    static seq_t *terms = NULL;
    static size_t terms_cnt = 0;
    static size_t terms_cap = 0;
    static uint64_t candidate = 2; /* next integer to test */

    /* Generate terms until we have at least n of them. */
    while (terms_cnt < n) {
        if (is_candidate(candidate)) {
            /* Ensure capacity for the new term. */
            if (terms_cnt == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *new_ptr = (seq_t *)realloc(terms,
                                                  new_cap * sizeof(seq_t));
                terms = new_ptr;
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = (seq_t)candidate;
        }
        ++candidate;
    }

    return terms[n - 1];
}
