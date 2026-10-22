/*
 * A358489  Numbers k such that phi(k) = 13! where phi is the Euler totient
 *          function (A000010).
 * https://oeis.org/A358489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A358489: Numbers k such that φ(k) = 13! (6227020800) */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Simple trial division primality test – sufficient for numbers ≤ 6e9 */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Comparator for qsort on seq_t values */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

/* Depth‑first search over the candidate primes */
static void dfs(const uint64_t *primes, size_t prime_cnt, size_t pos,
                uint64_t R, seq_t k,
                seq_t **solutions, size_t *sol_cnt, size_t *sol_cap)
{
    if (R == 1) {
        /* Record solution */
        if (*sol_cnt == *sol_cap) {
            *sol_cap = (*sol_cap == 0) ? 16 : *sol_cap * 2;
            *solutions = (seq_t *)realloc(*solutions, *sol_cap * sizeof(seq_t));
        }
        (*solutions)[*sol_cnt] = k;
        ++(*sol_cnt);
        return;
    }
    if (pos == prime_cnt) return;

    for (size_t i = pos; i < prime_cnt; i++) {
        uint64_t p = primes[i];
        if (R % (p - 1) != 0) continue;               /* (p-1) must divide R */
        uint64_t R0   = R;
        uint64_t factor = p - 1;                       /* (p-1)*p^(e-1) */
        seq_t k_curr = k;
        while (R0 % factor == 0) {
            k_curr = k_curr * (seq_t)p;
            uint64_t newR = R0 / factor;
            dfs(primes, prime_cnt, i + 1, newR, k_curr,
                solutions, sol_cnt, sol_cap);
            /* Next exponent: multiply factor by p, but stop if it would
               exceed R0 (no further contribution possible). */
            if (factor > R0 / p) break;
            factor *= p;
        }
    }
}

/* Generate all solutions for φ(k) = 13! and sort them */
static void generate_all(seq_t **solutions, size_t *sol_cnt, size_t *sol_cap)
{
    const uint64_t M = 6227020800ULL;          /* 13! */

    /* Prime factorization of M : 2^10 * 3^5 * 5^2 * 7 * 11 * 13 */
    const uint64_t pf[6] = {2, 3, 5, 7, 11, 13};
    const int ep[6]     = {10, 5, 2, 1, 1, 1};

    /* Generate all divisors of M (at most 1584) */
    uint64_t divs[2000];
    size_t div_cnt = 1;
    divs[0] = 1;
    for (int i = 0; i < 6; ++i) {
        uint64_t p = pf[i];
        int e = ep[i];
        size_t cur = div_cnt;
        uint64_t pow = 1;
        for (int k = 1; k <= e; ++k) {
            pow *= p;
            for (size_t j = 0; j < cur; ++j)
                divs[div_cnt++] = divs[j] * pow;
        }
    }

    /* Candidate primes p = d + 1 where d | M and p is prime */
    uint64_t primes[2000];
    size_t primes_cnt = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t p = divs[i] + 1;
        if (p > 1 && is_prime_uint64(p))
            primes[primes_cnt++] = p;
    }

    /* Allocate array for solutions */
    *sol_cap   = 16;
    *sol_cnt   = 0;
    *solutions = (seq_t *)malloc(*sol_cap * sizeof(seq_t));

    /* Enumerate all solutions */
    dfs(primes, primes_cnt, 0, M, (seq_t)1, solutions, sol_cnt, sol_cap);

    /* Sort the results */
    qsort(*solutions, *sol_cnt, sizeof(seq_t), cmp_seq);

    /* Remove duplicates (should not happen, but keep safe) */
    size_t uniq = 0;
    for (size_t i = 0; i < *sol_cnt; ++i) {
        if (uniq == 0 || (*solutions)[i] != (*solutions)[uniq - 1])
            (*solutions)[uniq++] = (*solutions)[i];
    }
    *sol_cnt = uniq;
}

/* Public function requested by the problem statement */
seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t *solutions = NULL;
    static size_t sol_cnt   = 0;
    static size_t sol_cap   = 0;

    if (!ready) {
        generate_all(&solutions, &sol_cnt, &sol_cap);
        ready = true;
    }

    if (n == 0 || (idx_t)n > sol_cnt)
        return 0;   /* out of range */

    return solutions[n - 1];
}
